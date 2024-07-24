
#include "rightpanel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>


rightPanel::rightPanel(QWidget *parent = nullptr) : QWidget(parent),brushSizeSlider(new QSlider(Qt::Horizontal)),
    undo(new QPushButton("Cofnij")), redo(new QPushButton("Przywróć")), nextButton(new QPushButton("Dalej")),
    dirtButton(new(QPushButton)),plantButton(new(QPushButton)),weedButton(new(QPushButton))
{
    this->setFixedWidth(250);
    QVBoxLayout *rightLayout = new QVBoxLayout(this);

    rightLayout->addWidget(brushSizeSlider);


    QHBoxLayout *colorLayout = new QHBoxLayout;
    dirtButton->setText("Ziemia");
    plantButton->setText("Roślina");
    weedButton->setText("Chwast");
    colorLayout->addWidget(dirtButton);
    colorLayout->addWidget(plantButton);
    colorLayout->addWidget(weedButton);
    rightLayout->addLayout(colorLayout);

    rightLayout->addStretch();

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(undo);
    hLayout->addWidget(redo);
    rightLayout->addLayout(hLayout);
    rightLayout->addWidget(nextButton);

    connect(dirtButton, &QPushButton::clicked, this, &rightPanel::colorClicked);
    connect(plantButton, &QPushButton::clicked, this, &rightPanel::colorClicked);
    connect(weedButton, &QPushButton::clicked, this, &rightPanel::colorClicked);


    connect(undo, &QPushButton::clicked, this, &rightPanel::undoSignal);
    connect(redo, &QPushButton::clicked, this, &rightPanel::redoSignal);

}

rightPanel::~rightPanel()
{

}

rightPanel::colorClicked()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button) {
        emit colorSignal(button->text());
    }
}

