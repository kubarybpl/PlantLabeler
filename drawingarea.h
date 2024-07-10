
#ifndef DRAWINGAREA_H
#define DRAWINGAREA_H


#include <QWidget>


class drawingArea : public QWidget
{
    Q_OBJECT
public:
    explicit drawingArea(QWidget *parent = nullptr);
    void setImage(const QString &imagePaths);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void drawLineTo(const QPoint &endPoint);


private:
    bool isDrawing;
//    QGraphicsPathItem *currentPath;
//    QGraphicsPixmapItem *image;
//    QStack<QGraphicsItem*> undoStack;
//    QStack<QGraphicsItem*> redoStack;
    int myPenWidth;
    QColor myPenColor;

    QPoint lastPoint;
    bool modified;

    QImage backgroundImage;
    QImage drawingImage;

signals:

};

#endif // DRAWINGAREA_H
