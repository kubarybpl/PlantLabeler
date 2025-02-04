#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QComboBox>
#include <QIcon>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow),
    graphicsView(new QGraphicsView(this)), zoom(1.0),leftWidget(new leftPanel(this)),
    toolBox(new rightPanel(this)), neuralDialog(new neuralNetworkDialog)
{
    ui->setupUi(this);

    qApp->installEventFilter(this);


    QMenu *menu = new QMenu(this);
    menu = menuBar()->addMenu(tr("&Doucz sieć"));
    QAction *newAct = new QAction(tr("&Uruchom interfejs"), this);
    connect(newAct, &QAction::triggered, this, &MainWindow::openNeuralDialog);
    menu->addAction(newAct);

    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout;

    mainLayout->addWidget(leftWidget);

    graphicsView->setFocusPolicy(Qt::StrongFocus);
    graphicsView->setFocus();

    scene = new interactiveScene(graphicsView);

    graphicsView->setMinimumWidth(680);
    graphicsView->setMinimumSize(680,420);
    graphicsView->setScene(scene);
    setCursor();

    mainLayout->addWidget(graphicsView);
    mainLayout->addWidget(toolBox);

    mainLayout->setStretchFactor(graphicsView, 1);

    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);


    connect(leftWidget, &leftPanel::imageSelected, scene, &interactiveScene::setImageItem);

    connect(scene, &interactiveScene::changeCursor, this, &MainWindow::setCursor);

    connect(toolBox, &rightPanel::sizeChanged, scene, &interactiveScene::setBrushSize);
    connect(toolBox, &rightPanel::opacityChanged, scene, &interactiveScene::changeMaskOpacity);
    connect(toolBox, &rightPanel::colorSignal, scene, &interactiveScene::setColor);
    connect(toolBox, &rightPanel::inferenceSignal, scene, &interactiveScene::inference);

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
    // Zoom in/out view
    if (event->modifiers() & Qt::ControlModifier) {
        const double scaleFactor = 1.15;
        if (event->angleDelta().y() > 0 && zoom <= 10.0) {
            graphicsView->scale(scaleFactor, scaleFactor); // Zoom in
            zoom *= scaleFactor;
            event->accept();
            setCursor();
            return;
        } else if(zoom >= 0.1) {
            graphicsView->scale(1.0 / scaleFactor, 1.0 / scaleFactor); // Zoom out
            zoom /= scaleFactor;
            event->accept();
            setCursor();
            return;
        }
    // Increse/decrease size of the brush
    } else if(event->modifiers() & Qt::ShiftModifier){
        int penSize = scene->getPen()->width();
        if (event->angleDelta().y() > 0) {
            toolBox->brushSliderChanged(penSize + 1);
            event->accept();
            return;
        } else if(zoom >= 0.1) {
            toolBox->brushSliderChanged(penSize - 1);
            event->accept();
            return;
        }
    }
    else{
        QMainWindow::wheelEvent(event);
    }
}

void MainWindow::openNeuralDialog()
{
    neuralDialog->exec();
    qDebug() << "klikneło w menu";
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

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type()==QEvent::KeyPress) {
        qDebug() << "przycisk z mainwindow";
        QKeyEvent* key = static_cast<QKeyEvent*>(event);
        if ((key->key()==Qt::Key_Z)) {
            scene->setVisibility("Ukryj maskę [z]");
        } else if((key->key()==Qt::Key_X)){
            scene->setVisibility("Ukryj tło [x]");
        }
        return true;
    }
    else if(event->type()==QEvent::KeyRelease) {
        qDebug() << "przycisk z mainwindow";

        QKeyEvent* key = static_cast<QKeyEvent*>(event);
        if ((key->key()==Qt::Key_Z)) {
            scene->setVisibility("Pokaż maskę [z]");
        } else if((key->key()==Qt::Key_X)){
            scene->setVisibility("Pokaż tło [x]");
        }
        return true;
    }
    else {
        return QObject::eventFilter(obj, event);
    }
    return false;
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
