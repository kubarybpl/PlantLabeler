
#include "rightpanel.h"


rightPanel::rightPanel(QWidget *parent = nullptr) : QWidget(parent),brushSizeSlider(new QSlider(Qt::Horizontal)),
    undo(new QPushButton("Cofnij")), redo(new QPushButton("Przywróć")), nextButton(new QPushButton("Dalej")), previousButton(new QPushButton("Wstecz")),
    dirtButton(new(QPushButton)("Ziemia")),plantButton(new(QPushButton)("Roślina")),weedButton(new(QPushButton)("Chwast")), maskVisibiltyButton(new(QPushButton)),
    backgroundVisibilityButton(new(QPushButton)), rightLayout(new QVBoxLayout(this)),visibilityButtonsLayout(new QHBoxLayout),
    colorLayout(new QHBoxLayout), undoRedoLayout(new QHBoxLayout), nextImageLayout(new QHBoxLayout)
{
    setupUI();


    connect(brushSizeSlider, &QSlider::valueChanged, this, &rightPanel::sliderChanged);

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

rightPanel::setupUI()
{
    this->setFixedWidth(250);


    //// Slider
    ///
    brushSizeSlider->setStyleSheet(R"(
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
)");
    brushSizeSlider->setOrientation(Qt::Horizontal);
    brushSizeSlider->setTickPosition(QSlider::TicksBothSides);
    brushSizeSlider->setTickInterval(5);

    brushSizeSlider->setSliderPosition(10);
    brushSizeSlider->setMinimum(1);
    brushSizeSlider->setMaximum(50);

    rightLayout->addWidget(brushSizeSlider);

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

    maskVisibiltyButton->setText("Ukryj maskę");
    maskVisibiltyButton->setStyleSheet(buttonStyle);

    backgroundVisibilityButton->setText("Ukryj tło");
    backgroundVisibilityButton->setStyleSheet(buttonStyle);

    visibilityButtonsLayout->addWidget(maskVisibiltyButton);
    visibilityButtonsLayout->addWidget(backgroundVisibilityButton);

    rightLayout->addLayout(visibilityButtonsLayout);


    dirtButton->setStyleSheet(buttonStyle);
//    dirtButton->setText("Ziemia");

    plantButton->setStyleSheet(buttonStyle);
//    plantButton->setText("Roślina");

    weedButton->setStyleSheet(buttonStyle);
//    weedButton->setText("Chwast");

    colorLayout->addWidget(dirtButton);
    colorLayout->addWidget(plantButton);
    colorLayout->addWidget(weedButton);

    rightLayout->addLayout(colorLayout);
    rightLayout->addStretch();


    undo->setStyleSheet(buttonStyle);
    redo->setStyleSheet(buttonStyle);
    undoRedoLayout->addWidget(undo);
    undoRedoLayout->addWidget(redo);
    rightLayout->addLayout(undoRedoLayout);


    nextButton->setStyleSheet(buttonStyle);
    previousButton->setStyleSheet(buttonStyle);
    nextImageLayout->addWidget(previousButton);
    nextImageLayout->addWidget(nextButton);
    nextImageLayout->addStrut(50);
    rightLayout->addLayout(nextImageLayout);









}

rightPanel::colorClicked()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button) {
        emit colorSignal(button->text());
    }
}

rightPanel::nextClicked()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if(button->text() =="Dalej")
        emit nextButtonClicked();
    else
        emit previousButtonClicked();
}

rightPanel::toggleView()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if(button){
        emit visibilitySignal(button->text());
    }

}

rightPanel::changeButton(QString &msg)
{
    if(msg == "Ukryj maskę") maskVisibiltyButton->setText("Pokaż maskę");
            else if(msg == "Pokaż maskę") maskVisibiltyButton->setText("Ukryj maskę");
            else if(msg == "Ukryj tło") backgroundVisibilityButton->setText("Pokaż tło");
            else if(msg == "Pokaż tło") backgroundVisibilityButton->setText("Ukryj tło");
}

