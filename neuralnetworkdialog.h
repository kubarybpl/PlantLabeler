
/**
 * @file neuralnetworkdialog.h
 * @brief Definition of the neuralNetworkDialog class which extends QDialog class for providing model fine-tunee and choose interface.
 */

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

// Namespace issues - Qt slots and torch slots
#undef slots
#include <torch/script.h>
#include<torch/torch.h>
#define slots Q_SLOTS

#define WIDTH 128
#define HEIGHT 128

/**
 * @class neuralNetworkDialog
 * @brief The neuralNetworkDialog class provides QDialog with tools to fine-tune and chose model.
 *
 * This class contains interface elements such as output area (logs), buttons to chose model and button to fine-tune model.
 */
class neuralNetworkDialog : public QDialog
{
    Q_OBJECT
public:
    /**
     * @brief Constructs a neuralNetworkDialog widget.
     */
    neuralNetworkDialog();

private:
    /**
     * @brief creates class GUI.
     */
    void setupUI();

    /**
     * @brief runs the fine-tune script.
     */
    void runButtonClicked();

    QVBoxLayout *layout;                ///< Main layout
    QHBoxLayout *choseNetworkLayout;    ///< Layout containing buttons to chose NN.
    QProcess *process;                  ///< Process instance to handle I/O.
    QPushButton *scriptButton;          ///< Button that runs python script used to detect new masks and fine-tune.
    QPushButton *unetButton;            ///< Button to load unet network.
    QPushButton *mobilenetButton;       ///< Button to load mobilenet network.
    QPlainTextEdit *outputArea;         ///< Text area to write external process output.

private slots:
    /**
     * @brief Print out external process output in QPlainTextEdit
     */
    void handleProcessOutput();

    /**
     * @brief Handle external script finish.
     * @param exitCode from external process.
     */
    void handleScriptFinish(int exitCode);

    /**
     * Activated on QPushButton::clicked. Sends text from button as QString.
     *
     * @brief emits networkSignal signal to load chosen model.
     */
    void changeNetwork();

signals:
    /**
     * @brief networkSignal contains information about model chosen to be loaded.
     * @param network QString containing network name.
     */
    void networkSignal(QString network);

};

#endif // NEURALNETWORKDIALOG_H
