
#ifndef NEURALNETWORKDIALOG_H
#define NEURALNETWORKDIALOG_H


#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QProcess>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

#include <opencv2/opencv.hpp>

#undef slots
#include <torch/script.h>
#include<torch/torch.h>
#define slots Q_SLOTS

#define WIDTH 128
#define HEIGHT 128

class neuralNetworkDialog : public QDialog
{
    Q_OBJECT
public:
    neuralNetworkDialog();

private:
    void setupUI();
    void buttonClicked();
    void learnButtonClicked();
    void runButtonClicked();
    void loadButtonClicked();

    QWidget *widget;
    QVBoxLayout *layout;
    QLabel *label;
    QProcess *process;
    QPushButton *learnButton;
    QPushButton *scriptButton;
private slots:
    void handleProcessOutput();
    void handleScriptFinished(int exitCode);

};

#endif // NEURALNETWORKDIALOG_H
