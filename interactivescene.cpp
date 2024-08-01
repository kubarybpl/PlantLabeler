#include "interactivescene.h"
#include <QPainter>
#include <qdebug.h>
#include <QString>
#include <QImage>

interactiveScene::interactiveScene(QObject *parent = nullptr)
    : QGraphicsScene(parent), isDrawing(0), image(nullptr),  frontItem(nullptr),myPenWidth(5), myPenColor(Qt::red),
    modified(false)
{}

void interactiveScene::setImageItem(const QString &imagePath)
{
    qDebug() << imagePath;
    this->clear();
    currentPath = imagePath;

    if(imagePath.lastIndexOf("mask_") != -1){
        qDebug() << "AAAAaaaaAAAA";
    }
    else {
        QPixmap pixmapBackground(imagePath);
        image = this->addPixmap(QPixmap(imagePath));
        this->setSceneRect(pixmapBackground.rect());

        front = QPixmap(this->width(),this->height());
        front.fill(Qt::transparent);
        frontItem = this->addPixmap(front);
        frontItem->setOpacity(0.3);
    }
}

void interactiveScene::setColor(const QString color)
{
    if (color == "Ziemia") {
        myPenColor = QColor(Qt::transparent);
    } else if (color == "Roślina") {
        myPenColor = QColor(Qt::green);
    } else if (color == "Chwast") {
        myPenColor = QColor(Qt::red);
    }
}

void interactiveScene::saveMask()
{
    QString path = currentPath;
    qsizetype last = currentPath.lastIndexOf("/", -1);

    QImage image = front.toImage();
    QImage maskImage(front.size(), QImage::Format_Indexed8);
    QVector<QRgb> colorTable{QColor(Qt::transparent).rgba(), QColor(Qt::green).rgb(), QColor(Qt::red).rgb()};
    maskImage.setColorTable(colorTable);

    maskImage.fill(Qt::transparent);
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

    if(maskImage.save(path.insert(last + 1, "mask_"))) qDebug() << "zapisało";
//    if(front.save(path.insert(last + 1, "mask_"))) qDebug() << "zapisało";

}

void interactiveScene::nextImage()
{
    if(image){
        saveMask();
    }
}

void interactiveScene::previousImage()
{
    qDebug() << "asdadada";
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
//        frontItem->setPixmap(item);
        undoStack.push_back(front);
        front = item;
        frontItem->setPixmap(front);
    }
}

void interactiveScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && image){
        modified = true;
        undoStack.push_back(front);

        QPainter painter(&front);
        painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        if(myPenColor == QColor(Qt::transparent))
            painter.setCompositionMode(QPainter::CompositionMode_Clear);
        else
            painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

        painter.drawPoint(event->scenePos());
        frontItem->setPixmap(front);

        lastPoint = event->scenePos();
        isDrawing = true;
    }
}

void interactiveScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if((event->buttons() & Qt::LeftButton) && isDrawing){
        QPainter painter(&front);
        painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        if(myPenColor == QColor(Qt::transparent))
            painter.setCompositionMode(QPainter::CompositionMode_Clear);
        else
            painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

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
