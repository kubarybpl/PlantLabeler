
#include "drawingarea.h"
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>


drawingArea::drawingArea(QWidget *parent)
    : QWidget{parent}, isDrawing(0), myPenWidth(5), myPenColor(Qt::black),
    modified(false)
{
    this->setMinimumSize(400,300);

}

void drawingArea::setImage(const QString &imagePath)
{
    backgroundImage.load(imagePath);

    modified = false;
    drawingImage = QImage(backgroundImage.size(), QImage::Format_ARGB32);
    drawingImage.fill(Qt::transparent);

    update();

    QPainter painter(this);
}


void drawingArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        lastPoint = event->position().toPoint();
        isDrawing = true;
    }
}

void drawingArea::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && isDrawing)
        drawLineTo(event->position().toPoint());

}

void drawingArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && isDrawing) {
        drawLineTo(event->position().toPoint());
        isDrawing = false;
    }
}

void drawingArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if (!backgroundImage.isNull()) {
        painter.drawImage(0, 0, backgroundImage);
    }
    if (!drawingImage.isNull()) {
        painter.drawImage(0, 0, drawingImage);
    }

//    QPainter painter(this);
//    QRect dirtyRect = event->rect();
//    painter.drawImage(dirtyRect, drawingImage, dirtyRect);
}


void drawingArea::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&drawingImage);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);
    modified = true;

    int rad = (myPenWidth / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized()
               .adjusted(-rad, -rad, +rad, +rad));
    lastPoint = endPoint;
}
