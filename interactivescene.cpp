
#include "interactivescene.h"

interactiveScene::interactiveScene(QObject *parent = nullptr)
    : QGraphicsScene(parent), isDrawing(0), currentPath(nullptr), image(nullptr), myPenWidth(5), myPenColor(Qt::black),
    modified(false)
{

}

void interactiveScene::setImageItem(QGraphicsPixmapItem *item)
{

    image = item;
//    item->setVisible(false);
}

//void interactiveScene::undo()
//{
//    if(!undoStack.isEmpty()){
//        QGraphicsItem *item = undoStack.pop();
//        redoStack.push(item);
//        removeItem(item);
//    }
//}

//void interactiveScene::redo()
//{
//    if(!redoStack.isEmpty()){
//        QGraphicsItem *item = redoStack.pop();
//        undoStack.push(item);
//        addItem(item);
//    }
//}

//void interactiveScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
//{
//    if(image != nullptr){
//        if(event->button() == Qt::LeftButton && image->contains(event->scenePos())){
//            lastPoint = event->position().toPoint();
//            isDrawing = true;
//        }
//    }
//}

//void interactiveScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
//{
//    if(isDrawing && currentPath && image->contains(event->scenePos())){
//        drawLineTo(event->position().toPoint());
//    }

//}

//void interactiveScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
//{
//    if(isDrawing && (event->button() == Qt::LeftButton)){
//        drawLineTo(event->position().toPoint());
//        isDrawing = false;
//    }
//}

//void interactiveScene::paintEvent(QPaintEvent *event)
//{
//    QPainter painter(this);
//    QRect dirtyRect = event->rect();
//    painter.drawImage(dirtyRect, image, dirtyRect);
//}

//void interactiveScene::drawLineTo(const QPoint &endPoint)
//{
//    QPainter painter(&image);
//    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
//                        Qt::RoundJoin));
//    painter.drawLine(lastPoint, endPoint);
//    modified = true;

//    int rad = (myPenWidth / 2) + 2;
//    update(QRect(lastPoint, endPoint).normalized()
//               .adjusted(-rad, -rad, +rad, +rad));
//    lastPoint = endPoint;
//}

/////////////////////////////////////////////////

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
