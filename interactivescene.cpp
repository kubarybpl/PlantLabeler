#include "interactivescene.h"
#include <QPainter>
#include <qdebug.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QString>

using namespace cv;

interactiveScene::interactiveScene(QObject *parent = nullptr)
    : QGraphicsScene(parent), isDrawing(0), image(nullptr),  frontItem(nullptr),myPenWidth(5), myPenColor(Qt::red),
    modified(false)
{}

void interactiveScene::setImageItem(const QString &imagePath)
{
    qDebug() << imagePath;
    this->clear();
    currentPath = imagePath;
    QPixmap pixmapBackground(imagePath);
    image = this->addPixmap(QPixmap(imagePath));
    this->setSceneRect(pixmapBackground.rect());

    front = QPixmap(this->width(),this->height());
    front.fill(Qt::transparent);
    frontItem = this->addPixmap(front);
    frontItem->setOpacity(0.3);

}

void interactiveScene::setColor(const QString color)
{
    if (color == "Ziemia") {
        myPenColor = QColor(121, 92, 50);
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

    if(front.save(path.insert(last + 1, "mask_"))) qDebug() << "zapisało";

    qDebug() << path;
//    qDebug() << last;

}

void interactiveScene::nextImage()
{
    saveMask();

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
