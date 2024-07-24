#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QComboBox>
#include <leftpanel.h>
#include <rightpanel.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), graphicsView(new QGraphicsView(this)), scene(new interactiveScene(this))
{
    ui->setupUi(this);

    //środek
    //Tworzenie centralnego widgetu
    QWidget *centralWidget = new QWidget(this);
    //Tworzenie Layoutu i dodawanie do niego widgetu
    QHBoxLayout *mainLayout = new QHBoxLayout;

    leftPanel *leftWidget = new leftPanel(this);
    rightPanel *toolBox= new rightPanel(this);

    mainLayout->addWidget(leftWidget);

    mainLayout->addWidget(graphicsView);

    mainLayout->addWidget(toolBox);

    graphicsView->setScene(scene);
//    graphicsView->setScene(nullptr);

    mainLayout->setStretchFactor(graphicsView, 1);

    //Dodawanie do Widgetu Layoutu
    centralWidget->setLayout(mainLayout);
    //Ustawianie
    setCentralWidget(centralWidget);

    connect(leftWidget, &leftPanel::imageSelected, scene, &interactiveScene::setImageItem);

    connect(toolBox, &rightPanel::colorSignal,  scene, &interactiveScene::setColor);

    connect(toolBox, &rightPanel::undoSignal, scene, &interactiveScene::undo);
    connect(toolBox, &rightPanel::redoSignal, scene, &interactiveScene::redo);

}
// Definicja slotu displayImage
MainWindow::displayImage(const QString &imagePath) {
//    scene->clear();
//    QPixmap image(imagePath);
//    QGraphicsPixmapItem *item = scene->addPixmap(image);
//    scene->setImageItem(item);
//    scene->setSceneRect(image.rect());

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::wheelEvent(QWheelEvent *event) {
    if (event->modifiers() & Qt::ControlModifier) {
        const double scaleFactor = 1.15; // Define the scale factor
        if (event->angleDelta().y() > 0) {
            graphicsView->scale(scaleFactor, scaleFactor); // Zoom in

        } else {
            graphicsView->scale(1.0 / scaleFactor, 1.0 / scaleFactor); // Zoom out
        }
        event->accept();
    } else {
//        QGraphicsView::wheelEvent(event); // Pass the event to the base class
    }

}
