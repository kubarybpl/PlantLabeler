
#include "neuralnetworkdialog.h"


#include <vector>
#include <string>
#include <utility>
#include <QFile>
#include <QStringList>
#include <torch/optim/adam.h>
#include "dataset.h"


neuralNetworkDialog::neuralNetworkDialog()
{
    process = new QProcess(this);
    setupUI();
}

void neuralNetworkDialog::setupUI()
{
    layout = new QVBoxLayout();
    this->resize(450,350);

    outputArea = new QPlainTextEdit(this);
    outputArea->setReadOnly(true);
    layout->addWidget(outputArea);

    this->setLayout(layout);

    scriptButton = new QPushButton("Test QProcess");
    layout->addWidget(scriptButton);

    choseNetworkLayout= new QHBoxLayout();
    unetButton = new QPushButton("UNet");
    mobilenetButton = new QPushButton("MobileNet");
    choseNetworkLayout->addWidget(unetButton);
    choseNetworkLayout->addWidget(mobilenetButton);

    layout->addLayout(choseNetworkLayout);


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

    scriptButton->setStyleSheet(buttonStyle);
    mobilenetButton->setStyleSheet(buttonStyle);
    unetButton->setStyleSheet(buttonStyle);

    connect(scriptButton, &QPushButton::clicked, this, &neuralNetworkDialog::runButtonClicked);

    connect(mobilenetButton, &QPushButton::clicked, this, &neuralNetworkDialog::changeNetwork);
    connect(unetButton, &QPushButton::clicked, this, &neuralNetworkDialog::changeNetwork);

    connect(process, &QProcess::readyReadStandardOutput,this, &neuralNetworkDialog::handleProcessOutput);

    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &neuralNetworkDialog::handleScriptFinish);

}

void neuralNetworkDialog::runButtonClicked()
{
    // Run the script
    QString scriptPath("E:/One Drive/OneDrive - Politechnika Warszawska/Magisterka/PlantLabeler/Python/main.py");
    QStringList scriptList;
    scriptList << scriptPath << "--data_dir" << "E:/One Drive/OneDrive - Politechnika Warszawska/Magisterka/test" ;
    scriptList << "--model_path" << "E:/One Drive/OneDrive - Politechnika Warszawska/Magisterka/model/" ;
    scriptList << "--csv_path" << "E:/One Drive/OneDrive - Politechnika Warszawska/Magisterka/PlantLabeler/Python/data.csv" ;
    scriptList << "--validation_dir" << "C:/Users/Kuba/Magisterka/Dataset/Test" ;
    process->start("C:/anaconda3/envs/PyTorch/python", {scriptList});

    if(process->state() == QProcess::Running){
        outputArea->clear();
        outputArea->appendPlainText("Skrypt jest uruchomiony");
        return;
    }

    if(!QFile::exists(scriptPath)){
        outputArea->appendPlainText("Błąd: Nie znaleziono pliku skryptu!");
        return;
    }
}


void neuralNetworkDialog::handleProcessOutput()
{
    // Reading and writing process output
    QString output = process->readAllStandardOutput();
    if(!output.isEmpty()){
        outputArea->appendPlainText(output);
    }
}


void neuralNetworkDialog::handleScriptFinish(int exitCode)
{
    if(exitCode == 0){
        outputArea->appendPlainText("Zakończono wykonywanie skryptu");
    } else {
        outputArea->appendPlainText("Błąd");
        outputArea->appendPlainText(process->readAllStandardError());
    }
}

void neuralNetworkDialog::changeNetwork()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if(button->text() == "UNet"){
        emit networkSignal("unet_init.pt");
    }
    else if(button->text() == "MobileNet"){
        emit networkSignal("mobilenet_init.pt");
    }
}
