#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QComboBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow),
    graphicsView(new QGraphicsView(this)), zoom(1.0),leftWidget(new leftPanel(this)),
    toolBox(new rightPanel(this))
{
    ui->setupUi(this);


    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout;

    mainLayout->addWidget(leftWidget);

    scene = new interactiveScene(graphicsView);
    graphicsView->setScene(scene);
    setCursor();

    mainLayout->addWidget(graphicsView);

    mainLayout->addWidget(toolBox);

    mainLayout->setStretchFactor(graphicsView, 0.8);

    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    connect(leftWidget, &leftPanel::imageSelected, scene, &interactiveScene::setImageItem);

    connect(scene, &interactiveScene::changeCursor, this, &MainWindow::setCursor);

    connect(toolBox, &rightPanel::sliderChanged, scene, &interactiveScene::setBrushSize);
    connect(toolBox, &rightPanel::colorSignal, scene, &interactiveScene::setColor);

    connect(toolBox, &rightPanel::visibilitySignal, scene, &interactiveScene::setVisibility);
    connect(scene, &interactiveScene::changeButton, toolBox, &rightPanel::changeButton);

    connect(toolBox, &rightPanel::undoSignal, scene, &interactiveScene::undo);
    connect(toolBox, &rightPanel::redoSignal, scene, &interactiveScene::redo);

    connect(toolBox, &rightPanel::nextButtonClicked, this, &MainWindow::nextImage);
    connect(toolBox, &rightPanel::previousButtonClicked, this, &MainWindow::previousImage);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::wheelEvent(QWheelEvent *event) {
    if (event->modifiers() & Qt::ControlModifier) {
        const double scaleFactor = 1.15;
        if (event->angleDelta().y() > 0) {
            graphicsView->scale(scaleFactor, scaleFactor); // Zoom in
            zoom *= scaleFactor;
        } else {
            graphicsView->scale(1.0 / scaleFactor, 1.0 / scaleFactor); // Zoom out
            zoom /= scaleFactor;
        }
        event->accept();
    }
    setCursor();
}

void MainWindow::nextImage()
{
    // Load actual path
    QString path = scene->getPath();
    if(!path.isEmpty()){
        // Get next path, load image, select file in tree
        QString nextFilePath = leftWidget->getNextFileFromTree(path);
        scene->nextImage(nextFilePath);
        leftWidget->selectModelInTree(nextFilePath);
    }
}

void MainWindow::previousImage()
{
    // Load actual path
    QString path = scene->getPath();
    if(!path.isEmpty()){
        // Get next path, load image, select file in tree
        QString nextFilePath = leftWidget->getNextFileFromTree(path, 1);
        scene->nextImage(nextFilePath);
        leftWidget->selectModelInTree(nextFilePath);
    }
}

void MainWindow::setCursor()
{
    int size = scene->getPen()->width() * zoom;
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(Qt::black);
    painter.setBrush(scene->getPen()->brush());
    painter.setOpacity(0.2);
    painter.drawEllipse(0, 0, size, size);
    painter.end();

    QCursor cursor(pixmap);
    graphicsView->setCursor(cursor);
}
