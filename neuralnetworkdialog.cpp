
#include "neuralnetworkdialog.h"
#include <QLabel>
#include <QVBoxLayout>

neuralNetworkDialog::neuralNetworkDialog()
{
    setupUI();
}

neuralNetworkDialog::setupUI()
{
    QVBoxLayout *layout = new QVBoxLayout();
    this->resize(450,350);
    QLabel *label = new QLabel(this);

    label->setFrameStyle(QFrame::Panel);
    label->setText("Tu będą informacje\no sieciach/danych");
    label->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    layout->addWidget(label);
    QWidget *widget = new QWidget(this);
    layout->addWidget(widget);
    this->setLayout(layout);

}
