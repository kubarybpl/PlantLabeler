
#include "mainwindow.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName(QString("Plant Labeler"));
    a.setWindowIcon(QIcon(":/icons/plant.png"));

    MainWindow w;
    w.setWindowTitle("Plant Labeler");
    w.show();
    return a.exec();
}
