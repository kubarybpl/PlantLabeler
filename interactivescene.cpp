#include "interactivescene.h"
#include <QPainter>
#include <qdebug.h>

interactiveScene::interactiveScene(QObject *parent = nullptr)
    : QGraphicsScene(parent), isDrawing(0), currentPath(nullptr), image(nullptr),  frontItem(nullptr),myPenWidth(5), myPenColor(Qt::red),
    modified(false)
{

}

void interactiveScene::setImageItem(const QString &imagePath)
{
    this->clear();
    QPixmap pixmapBackground(imagePath);
    image = this->addPixmap(QPixmap(imagePath));
    this->setSceneRect(pixmapBackground.rect());

    front = QPixmap(this->width(),this->height());
    front.fill(Qt::transparent);
    frontItem = this->addPixmap(front);
    frontItem->setOpacity(0.2);

}

void interactiveScene::setColor(const QString color)
{
    if (color == "Ziemia") {
        myPenColor = QColor(160,82,45);
    } else if (color == "Roślina") {
        myPenColor = QColor(Qt::green);
    } else if (color == "Chwast") {
        myPenColor = QColor(Qt::red);
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
    if(event->button() == Qt::LeftButton){
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
