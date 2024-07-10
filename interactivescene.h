
#ifndef INTERACTIVESCENE_H
#define INTERACTIVESCENE_H


#include <QObject>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPathItem>
#include <QStack>
#include <QGraphicsSceneWheelEvent>

class interactiveScene : public QGraphicsScene
{
    Q_OBJECT
public:
    interactiveScene(QObject *parent);
    void setImageItem(QGraphicsPixmapItem *item);
    void undo();
    void redo();
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

//    void paintEvent(QPaintEvent *event) override;
    void drawLineTo(const QPoint &endPoint);

private:
    bool isDrawing;
    QGraphicsPathItem *currentPath;
    QGraphicsPixmapItem *image;
    QStack<QGraphicsItem*> undoStack;
    QStack<QGraphicsItem*> redoStack;
    int myPenWidth;
    QColor myPenColor;

    QPoint lastPoint;
    bool modified;
};

#endif // INTERACTIVESCENE_H
