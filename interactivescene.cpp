#include "interactivescene.h"
#include <QPainter>
#include <qdebug.h>
#include <QString>
#include <QImage>
#include <QCursor>
#include <QGraphicsView>
#include <QFileInfo>
#include <QDir>


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#define WIDTH 128
#define HEIGHT 128

interactiveScene::interactiveScene(QGraphicsView *parent = nullptr)
    : QGraphicsScene(parent), isDrawing(0), image(nullptr), frontItem(nullptr),
    modified(false), pen(Qt::red, 10, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin),
    currentPath(QString()), opacity(0.3)
{
    try {
        model = torch::jit::load("C:/Users/Kuba/Magisterka/unet_init.pt");
    }
    catch (const c10::Error& e) {
        qDebug() << "error loading the model";
        qDebug() << e.what_without_backtrace();
        return;
    }
}

void interactiveScene::setImageItem(const QString &imagePath)
{
    // if image is modified save
    if(modified){
        saveMask();
        undoStack.clear();
        redoStack.clear();
        modified = 0;
    }
    qDebug() << imagePath;

    //setting screen and image
    this->clear();
    currentPath = imagePath;
    modified = 0;

    QPixmap pixmapBackground(imagePath);
    image = this->addPixmap(QPixmap(imagePath));
    this->setSceneRect(pixmapBackground.rect());

    //checking if mask already exists
    QFileInfo fileInfo(imagePath);
    QString fileName = fileInfo.fileName();
    QString fileDirectory = fileInfo.absolutePath();
    QString maskFilePath = fileDirectory + "/mask_" + fileName;
    QDir dir(fileInfo.absoluteFilePath());

    // if mask exists load it, otherwise make new transparent pixmap
    if(dir.exists(maskFilePath)) {
        QImage loadedMask(maskFilePath);

        // Re-create the mask based on indexes
        QImage coloredImage(loadedMask.size(), QImage::Format_ARGB32);
        for (int y = 0; y < loadedMask.height(); ++y) {
            for (int x = 0; x < loadedMask.width(); ++x) {
                int index = loadedMask.pixelIndex(x, y);
                QColor color;
                if (index == 2) {
                    color = Qt::green;
                } else if (index == 1) {
                    color = Qt::red;
                } else {
                    color = Qt::transparent;
                }
                coloredImage.setPixelColor(x, y, color);
            }
        }

        front = QPixmap::fromImage(coloredImage);
        qDebug() << "Wczytano maskę: " << maskFilePath;
    }
    else{
        front = QPixmap(this->width(),this->height());
        front.fill(Qt::transparent);
    }

    frontItem = this->addPixmap(front);
    frontItem->setOpacity(opacity);

}

void interactiveScene::inference()
{
    if(image){
        // QImage to tensor
        torch::Tensor tensorImage;
        try{
            cv::Mat img = cv::imread(currentPath.toStdString(), cv::IMREAD_COLOR);
            cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
            tensorImage = torch::from_blob(img.data, {img.rows, img.cols, img.channels()}, torch::kByte);
            tensorImage = tensorImage.permute({2, 0, 1});
            tensorImage = tensorImage.to(torch::kFloat32) / 255.0;
            tensorImage = tensorImage.unsqueeze(0);
        }
        catch (const c10::Error& e){
            qDebug() << "error during inference\n";
            qDebug() << e.what_without_backtrace();
            return;
        }

        std::vector<torch::jit::IValue> inputs;
        inputs.push_back(tensorImage);

        model.eval();
        try {
            torch::Tensor output = model.forward(inputs).toTensor();
            output = output.squeeze(0);
            torch::Tensor argMaxClasses = output.argmax(0);

            int height = argMaxClasses.size(0);
            int width  = argMaxClasses.size(1);
            QImage coloredImage(width, height, QImage::Format_ARGB32);


            auto argMaxAcc = argMaxClasses.accessor<int64_t,2>();

            for (int h = 0; h < height; ++h) {
                for (int w = 0; w < width; ++w) {
                    int classId = argMaxAcc[h][w];

                    QColor color;
                    switch (classId) {
                    case 0:
                        // klasa 0 -> czerwony
                        color = Qt::green;
                        break;
                    case 1:
                        // klasa 1 -> zielony
                        color = Qt::transparent;
                        break;
                    case 2:
                        // klasa 2 -> przezroczysty
                        color = Qt::red;
                        break;
                    default:
                        color = Qt::transparent;
                        break;
                    }
                    coloredImage.setPixelColor(w, h, color);
                }
            }

            coloredImage.save("C:/Users/Kuba/output_qt.png");
            this->removeItem(frontItem);

            front = QPixmap::fromImage(coloredImage);
            frontItem = this->addPixmap(front);
            frontItem->setOpacity(opacity);

        } catch (const c10::Error& e) {
            qDebug() << "error during inference\n";
            qDebug() << e.what();
            return;
        }
    }
}

void interactiveScene::setColor(const QString color)
{
    if (color == "Ziemia") {
        pen.setColor(QColor(Qt::transparent));
    } else if (color == "Roślina") {
        pen.setColor(QColor(Qt::green));
    } else if (color == "Chwast") {
        pen.setColor(QColor(Qt::red));
    }
    setCursor();
}

void interactiveScene::saveMask()
{
    // Make QImage based on QPixmap with indices as colors represented by colorTable
    QString path = currentPath;
    qsizetype last = currentPath.lastIndexOf("/", -1);

    QImage image = front.toImage();
    QImage maskImage(front.size(), QImage::Format_Indexed8);

    QVector<QRgb> colorTable{QColor(Qt::transparent).rgba(), QColor(Qt::green).rgb(), QColor(Qt::red).rgb()};
    maskImage.setColorTable(colorTable);

    //saving image as a matrix with indexes
    maskImage.fill(0);
    for (int y = 0; y < maskImage.height(); ++y) {
        for (int x = 0; x < maskImage.width(); ++x) {
            QColor color = image.pixelColor(x, y);

            int index;
            if (color == QColor(Qt::green)) {
                index = 2;
            } else if (color == QColor(Qt::red)) {
                index = 1;
            } else {
                index = 0;
            }

            maskImage.setPixel(x, y, index);
        }
    }

    if(maskImage.save(path.insert(last + 1, "mask_")))
        qDebug() << "zapisało maskę: " << path;
}


void interactiveScene::nextImage(QString path)
{
    if(image){
        saveMask();
        undoStack.clear();
        redoStack.clear();
        setImageItem(path);
    }
}


void interactiveScene::previousImage(QString path)
{
    if(image){
        saveMask();
        undoStack.clear();
        redoStack.clear();
        setImageItem(path);
    }
}


QPen *interactiveScene::getPen()
{
    return &pen;
}


QString interactiveScene::getPath()
{
    return currentPath;
}


void interactiveScene::setVisibility(QString visibility)
{
    if(image){
        if (visibility == "Ukryj maskę [z]") frontItem->setVisible(0);
        else if (visibility == "Pokaż maskę [z]") frontItem->setVisible(1);
        else if (visibility == "Ukryj tło [x]") image->setVisible(0);
        else if (visibility == "Pokaż tło [x]") image->setVisible(1);
        emit changeButton(visibility);
    }
}


void interactiveScene::setCursor()
{
    emit changeCursor();
}


void interactiveScene::undo()
{
    if(!undoStack.isEmpty()){
        QPixmap item = undoStack.pop();
        redoStack.push(front);
        front = item;
        frontItem->setPixmap(front);
    }
}


void interactiveScene::redo()
{
    if(!redoStack.isEmpty()){
        QPixmap item = redoStack.pop();
        undoStack.push_back(front);
        front = item;
        frontItem->setPixmap(front);
    }
}

void interactiveScene::setBrushSize(int size)
{
    pen.setWidth(size);
    setCursor();
}

void interactiveScene::changeMaskOpacity(int value)
{
    opacity = value / 10.0;
    if(image){
        frontItem->setOpacity(opacity);
    }
}

void interactiveScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    if(event->button() == Qt::LeftButton && image){

        modified = true;
        // Pushing current state on stack
        undoStack.push_back(front);
        // Set painter
        QPainter painter(&front);
        painter.setPen(pen);
        // If color is set to be ground - set special mode to erase
        if(pen.color() == Qt::transparent)
            painter.setCompositionMode(QPainter::CompositionMode_Clear);
        else
            painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

        painter.drawPoint(event->scenePos());
        // Set painted element on QGraphicsPixmapItem
        frontItem->setPixmap(front);
        lastPoint = event->scenePos();

        isDrawing = true;
    }
}

void interactiveScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if((event->buttons() & Qt::LeftButton) && isDrawing){
        // Set painter
        QPainter painter(&front);
        painter.setPen(pen);
        // If color is set to be ground - set special mode to erase
        if(pen.color() == Qt::transparent)
            painter.setCompositionMode(QPainter::CompositionMode_Clear);
        else
            painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        // Draw line set painted element on QGraphicsPixmapItem
        painter.drawLine(lastPoint, event->scenePos().toPoint());
        frontItem->setPixmap(front);
        lastPoint = event->scenePos();
    }
}


void interactiveScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && isDrawing){
        frontItem->setPixmap(front);
        isDrawing = false;
        redoStack.clear();
    }
}

void interactiveScene::wheelEvent(QGraphicsSceneWheelEvent *event)
{
//    if(event->modifiers() & Qt::ControlModifier) {
//        event->accept();
//        return;
//    }
//    if(event->modifiers() & Qt::ShiftModifier){
//        event->accept();
//        return;
//    }
//    else QGraphicsScene::wheelEvent(event);
}

