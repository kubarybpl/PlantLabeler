#include "interactivescene.h"
#include <QPainter>

interactiveScene::interactiveScene(QObject *parent = nullptr)
    : QGraphicsScene(parent), isDrawing(0), currentPath(nullptr), image(nullptr),  frontItem(nullptr),myPenWidth(5), myPenColor(Qt::black),
    modified(false)
{

}

//void interactiveScene::setImageItem(QGraphicsPixmapItem *item)
//{

//    image = item;
//    //    item->setVisible(false);
//}

void interactiveScene::setImageItem(const QString &imagePath)
{
    this->clear();
    QPixmap pixmapBackground(imagePath);
//    QGraphicsPixmapItem *item = this->addPixmap(QPixmap(imagePath));
    image = this->addPixmap(QPixmap(imagePath));
    this->setSceneRect(pixmapBackground.rect());

    front = QPixmap(this->width(),this->height());
    front.fill(Qt::transparent);
    frontItem = this->addPixmap(front);

}


void interactiveScene::undo()
{

}

void interactiveScene::redo()
{

}

void interactiveScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
//    pixmap = frontItem->pixmap();
    QPainter painter(&front);
    painter.setPen(QPen(Qt::red, 5));
    painter.drawEllipse(event->scenePos(), 10, 10);
}

void interactiveScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

}

void interactiveScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    frontItem->setPixmap(front);
}


/*

void interactiveScene::undo()
{
    if(!undoStack.isEmpty()){
        QGraphicsItem *item = undoStack.pop();
        redoStack.push(item);
        removeItem(item);
    }
}

void interactiveScene::redo()
{
    if(!redoStack.isEmpty()){
        QGraphicsItem *item = redoStack.pop();
        undoStack.push(item);
        addItem(item);
    }
}

void interactiveScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(image != nullptr){
        if(event->button() == Qt::LeftButton && image->contains(event->scenePos())){
            currentPath = new QGraphicsPathItem();
            currentPath->setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
            QPainterPath path;
            path.moveTo(event->scenePos());

                        path.lineTo(event->scenePos());

            currentPath->setPath(path);
            addItem(currentPath);
            isDrawing = true;
        }
    }
}

void interactiveScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(isDrawing && currentPath && image->contains(event->scenePos())){
        QPainterPath path = currentPath->path();
        path.lineTo(event->scenePos());
        currentPath->setPath(path);
    }

}

void interactiveScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(isDrawing && (event->button() == Qt::LeftButton)){

//        QPainterPath path = currentPath->path();
//        path.moveTo(event->scenePos());
//        currentPath->setPath(path);

        QPainterPath path = currentPath->path();
        path.lineTo(event->scenePos());

        isDrawing = false;
        undoStack.push(currentPath);
    }
}
*/
