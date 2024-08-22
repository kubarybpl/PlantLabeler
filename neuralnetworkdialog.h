
#ifndef NEURALNETWORKDIALOG_H
#define NEURALNETWORKDIALOG_H


#include <QObject>
#include <QWidget>
#include <QDialog>

class neuralNetworkDialog : public QDialog
{
    Q_OBJECT
public:
    neuralNetworkDialog();

private:
    setupUI();
};

#endif // NEURALNETWORKDIALOG_H
