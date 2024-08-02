#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QComboBox>
#include <leftpanel.h>
#include <rightpanel.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow),
    graphicsView(new QGraphicsView(this)), zoom(1.0)
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

    scene = new interactiveScene(graphicsView);
    graphicsView->setScene(scene);
    setCursor();
//    graphicsView->setCursor(Qt::PointingHandCursor);
    mainLayout->addWidget(graphicsView);

    mainLayout->addWidget(toolBox);

    mainLayout->setStretchFactor(graphicsView, 1);

    //Dodawanie do Widgetu Layoutu
    centralWidget->setLayout(mainLayout);
    //Ustawianie
    setCentralWidget(centralWidget);

    connect(leftWidget, &leftPanel::imageSelected, scene, &interactiveScene::setImageItem);

//    connect(toolBox, &rightPanel::sliderChanged, this, &MainWindow::setCursor);


    connect(toolBox, &rightPanel::sliderChanged, scene, &interactiveScene::setBrushSize);
    connect(toolBox, &rightPanel::colorSignal, scene, &interactiveScene::setColor);
//    connect(toolBox, &rightPanel::sliderChanged, this, &MainWindow::setBrushSize);
//    connect(toolBox, &rightPanel::colorSignal, this, &MainWindow::setBrushColor);

    connect(toolBox, &rightPanel::visibilitySignal, scene, &interactiveScene::setVisibility);
    connect(scene, &interactiveScene::changeButton, toolBox, &rightPanel::changeButton);

    connect(toolBox, &rightPanel::undoSignal, scene, &interactiveScene::undo);
    connect(toolBox, &rightPanel::redoSignal, scene, &interactiveScene::redo);

    connect(toolBox, &rightPanel::nextButtonClicked, scene, &interactiveScene::nextImage);
    connect(toolBox, &rightPanel::previousButtonClicked, scene, &interactiveScene::previousImage);


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
            zoom *= scaleFactor;
        } else {
            graphicsView->scale(1.0 / scaleFactor, 1.0 / scaleFactor); // Zoom out
            zoom /= scaleFactor;
        }
        event->accept();
    } else {
//        QGraphicsView::wheelEvent(event); // Pass the event to the base class
    }
    setCursor();
}

void MainWindow::setCursor()
{
    int size = scene->getPen()->width() * zoom;
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);

    painter.setPen(Qt::NoPen);
    painter.setBrush(scene->getPen()->brush());
    painter.setOpacity(0.1);
    painter.drawEllipse(0, 0, size, size);
    painter.end();

    QCursor cursor(pixmap);
    graphicsView->setCursor(cursor);
}
