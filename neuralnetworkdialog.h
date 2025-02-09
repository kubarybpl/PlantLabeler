
#ifndef NEURALNETWORKDIALOG_H
#define NEURALNETWORKDIALOG_H


#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QProcess>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QPlainTextEdit>

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
    void runButtonClicked();
    void loadButtonClicked();

    QWidget *widget;
    QVBoxLayout *layout;
    QHBoxLayout *choseNetworkLayout;

//    QLabel *label;
    QProcess *process;
    QPushButton *learnButton;
    QPushButton *scriptButton;
    QPushButton *unetButton;
    QPushButton *mobilenetButton;
    QPlainTextEdit *outputArea;
private slots:
    void handleProcessOutput();
    void handleScriptFinished(int exitCode);
    void changeNetwork();

signals:
    void networkSignal(QString network);

};

#endif // NEURALNETWORKDIALOG_H
