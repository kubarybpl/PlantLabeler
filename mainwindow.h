
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include "interactivescene.h"
#include <QWheelEvent>
#include <drawingarea.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void wheelEvent(QWheelEvent *event) override;

private:
    Ui::MainWindow *ui;
    QGraphicsView *graphicsView;
    interactiveScene *scene;

private slots:
    displayImage(const QString &imagePath);
};

#endif // MAINWINDOW_H
