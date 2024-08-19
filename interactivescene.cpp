#include "interactivescene.h"
#include <QPainter>
#include <qdebug.h>
#include <QString>
#include <QImage>
#include <QCursor>
#include <QGraphicsView>
#include <QFileInfo>
#include <QDir>


interactiveScene::interactiveScene(QGraphicsView *parent = nullptr)
    : QGraphicsScene(parent), isDrawing(0), image(nullptr), frontItem(nullptr),
    modified(false), pen(Qt::red, 10, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin),
    currentPath(QString())
{}

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
    frontItem->setOpacity(0.3);

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
        if (visibility == "Ukryj maskę") frontItem->setVisible(0);
        else if (visibility == "Pokaż maskę") frontItem->setVisible(1);
        else if (visibility == "Ukryj tło") image->setVisible(0);
        else if (visibility == "Pokaż tło") image->setVisible(1);
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
