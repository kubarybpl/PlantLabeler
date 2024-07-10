
#include "rightpanel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>


rightPanel::rightPanel(QWidget *parent = nullptr) : QWidget(parent),brushSizeSlider(new QSlider(Qt::Horizontal)), colorComboBox(new QComboBox()),
    undo(new QPushButton("Cofnij")), redo(new QPushButton("Przywróć")), nextButton(new QPushButton("Dalej"))
{
    this->setFixedWidth(250);
    QVBoxLayout *rightLayout = new QVBoxLayout(this);

    rightLayout->addWidget(brushSizeSlider);
//    rightLayout->addWidget(colorComboBox);
    rightLayout->addStretch();

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(undo);
    hLayout->addWidget(redo);
    rightLayout->addLayout(hLayout);
    rightLayout->addWidget(nextButton);

    connect(undo, &QPushButton::clicked, this, &rightPanel::undoSignal);
    connect(redo, &QPushButton::clicked, this, &rightPanel::redoSignal);

}

rightPanel::~rightPanel()
{

}

