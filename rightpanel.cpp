
#include "rightpanel.h"


rightPanel::rightPanel(QWidget *parent = nullptr) : QWidget(parent), brushSizeSlider(new QSlider(Qt::Horizontal)), opacitySlider(new QSlider(Qt::Horizontal)),
    undo(new QPushButton("Cofnij")), redo(new QPushButton("Przywróć")), nextButton(new QPushButton("Dalej")), previousButton(new QPushButton("Wstecz")),
    dirtButton(new(QPushButton)("Ziemia")),plantButton(new(QPushButton)("Roślina")),weedButton(new(QPushButton)("Chwast")), maskVisibiltyButton(new(QPushButton)),
    backgroundVisibilityButton(new(QPushButton)), rightLayout(new QVBoxLayout(this)),visibilityButtonsLayout(new QHBoxLayout),
    colorLayout(new QHBoxLayout), inferenceLayout(new QHBoxLayout), undoRedoLayout(new QHBoxLayout), nextImageLayout(new QHBoxLayout),inferenceButton(new QPushButton("Predykcja")),
    labelBrush(new QLabel("Wielkość pędzla 10px", this)),labelOpacity(new QLabel("Przezroczystość: 30%", this))
{
    setupUI();

    connect(brushSizeSlider, &QSlider::valueChanged, this, &rightPanel::brushSliderChanged);
    connect(opacitySlider, &QSlider::valueChanged, this, &rightPanel::opacitySliderChanged);

    connect(inferenceButton, &QPushButton::clicked, this, &rightPanel::inferenceClicked);
    connect(dirtButton, &QPushButton::clicked, this, &rightPanel::colorClicked);
    connect(plantButton, &QPushButton::clicked, this, &rightPanel::colorClicked);
    connect(weedButton, &QPushButton::clicked, this, &rightPanel::colorClicked);

    connect(maskVisibiltyButton, &QPushButton::clicked, this, &rightPanel::toggleView);
    connect(backgroundVisibilityButton, &QPushButton::clicked, this, &rightPanel::toggleView);

    connect(undo, &QPushButton::clicked, this, &rightPanel::undoSignal);
    connect(redo, &QPushButton::clicked, this, &rightPanel::redoSignal);

    connect(nextButton, &QPushButton::clicked, this, &rightPanel::nextClicked);
    connect(previousButton, &QPushButton::clicked, this, &rightPanel::previousButtonClicked);

}

rightPanel::~rightPanel()
{

}

void rightPanel::setupUI()
{
    this->setFixedWidth(250);


    //// Slider
    ///
    QString sliderStyle = R"(
    QSlider::groove:horizontal {
        border: 1px solid #999999;
        height: 8px;
        background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #b1b1b1, stop:1 #c4c4c4);
        margin: 2px 0;
    }

    QSlider::handle:horizontal {
        background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b4b4b4, stop:1 #8f8f8f);
        border: 1px solid #5c5c5c;
        width: 12px;
        height: 10px;
        margin: -8px 0;
        border-radius: 3px;
    }
)";


    brushSizeSlider->setStyleSheet(sliderStyle);
    brushSizeSlider->setOrientation(Qt::Horizontal);
    brushSizeSlider->setTickPosition(QSlider::TicksBothSides);
    brushSizeSlider->setTickInterval(5);
    brushSizeSlider->setSliderPosition(10);
    brushSizeSlider->setMinimum(1);
    brushSizeSlider->setMaximum(50);

    labelBrush->setAlignment(Qt::AlignCenter);
    labelBrush->setStyleSheet("font-size: 12px;");

    rightLayout->addWidget(labelBrush);
    rightLayout->addWidget(brushSizeSlider);


    opacitySlider->setStyleSheet(sliderStyle);
    opacitySlider->setOrientation(Qt::Horizontal);
    opacitySlider->setTickPosition(QSlider::TicksBothSides);
    opacitySlider->setTickInterval(1);
    opacitySlider->setSliderPosition(3);
    opacitySlider->setMinimum(0);
    opacitySlider->setMaximum(10);

    labelOpacity->setAlignment(Qt::AlignCenter);
    labelOpacity->setStyleSheet("font-size: 12px;");

    rightLayout->addWidget(labelOpacity);
    rightLayout->addWidget(opacitySlider);

    //// Buttons
    ///
    QString buttonStyle = R"(
    QPushButton {
    background-color: #f0f0f0;
    border: 1px solid #cccccc;
    border-radius: 5px;
    padding: 5px;
    margin: 2px;
}
    QPushButton:hover {
    background-color: #e1e1e1;
})";

    maskVisibiltyButton->setText("Ukryj maskę [z]");
    maskVisibiltyButton->setStyleSheet(buttonStyle);

    backgroundVisibilityButton->setText("Ukryj tło [x]");
    backgroundVisibilityButton->setStyleSheet(buttonStyle);

    visibilityButtonsLayout->addWidget(maskVisibiltyButton);
    visibilityButtonsLayout->addWidget(backgroundVisibilityButton);

    rightLayout->addLayout(visibilityButtonsLayout);

    dirtButton->setStyleSheet(buttonStyle);
    plantButton->setStyleSheet(buttonStyle);
    weedButton->setStyleSheet(buttonStyle);
    inferenceButton->setStyleSheet(buttonStyle);

    colorLayout->addWidget(dirtButton);
    colorLayout->addWidget(plantButton);
    colorLayout->addWidget(weedButton);

    inferenceLayout->addWidget(inferenceButton);

    rightLayout->addLayout(colorLayout);
    rightLayout->addLayout(inferenceLayout);
    rightLayout->addStretch();


    undo->setStyleSheet(buttonStyle);
    redo->setStyleSheet(buttonStyle);
    undoRedoLayout->addWidget(undo);
    undoRedoLayout->addWidget(redo);
    rightLayout->addLayout(undoRedoLayout);


    nextButton->setStyleSheet(buttonStyle);
    previousButton->setStyleSheet(buttonStyle);
    nextImageLayout->addWidget(nextButton);
    nextImageLayout->addWidget(previousButton);
    nextImageLayout->addStrut(50);
    rightLayout->addLayout(nextImageLayout);
}

void rightPanel::colorClicked()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button) {
        emit colorSignal(button->text());
    }
}

void rightPanel::nextClicked()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if(button->text() =="Dalej")
        emit nextButtonClicked();
    else
        emit previousButtonClicked();
}

void rightPanel::toggleView()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if(button){
        emit visibilitySignal(button->text());
    }

}

void rightPanel::inferenceClicked()
{
    emit inferenceSignal();
}

void rightPanel::brushSliderChanged(int size)
{
    if(size <= brushSizeSlider->maximum() && size >= brushSizeSlider->minimum()){
        brushSizeSlider->setValue(size);
        labelBrush->setText(QString::fromStdString("Wielkość pędzla: " + std::to_string(size) + "px"));
        emit sizeChanged(size);
    }
}

void rightPanel::opacitySliderChanged(int value)
{
    labelOpacity->setText(QString::fromStdString("Przezroczystość " + std::to_string(100 - (value * 10)) + "%"));
    emit opacityChanged(value);
}

void rightPanel::changeButton(QString &msg)
{
    if(msg == "Ukryj maskę [z]") maskVisibiltyButton->setText("Pokaż maskę [z]");
            else if(msg == "Pokaż maskę [z]") maskVisibiltyButton->setText("Ukryj maskę [z]");
            else if(msg == "Ukryj tło [x]") backgroundVisibilityButton->setText("Pokaż tło [x]");
            else if(msg == "Pokaż tło [x]") backgroundVisibilityButton->setText("Ukryj tło [x]");
}

