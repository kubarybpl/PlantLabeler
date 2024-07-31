
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
    void setImageItem(const QString &imagePath);
    void undo();
    void redo();
    void setColor(QString color);
    void saveMask();
    void nextImage();

public slots:
    void setVisibility(QString visibility);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void drawLineTo(const QPoint &endPoint);

private:
    bool isDrawing;
    QString currentPath;
    QGraphicsPixmapItem *image;
    QGraphicsPixmapItem *frontItem;
    QPixmap front;
    QStack<QPixmap> undoStack;
    QStack<QPixmap> redoStack;
    int myPenWidth;
    QColor myPenColor;

    QPointF lastPoint;
    bool modified;

signals:
    void changeButton(QString &msg);
};

#endif // INTERACTIVESCENE_H
