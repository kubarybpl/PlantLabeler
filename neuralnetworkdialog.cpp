
#include "neuralnetworkdialog.h"


#include <vector>
#include <string>
#include <utility>
#include <QFile>

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
    label = new QLabel(this);

    label->setFrameStyle(QFrame::Panel);\
    label->setText("Tu będą informacje\no sieciach/danych");
    label->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    layout->addWidget(label);

    widget = new QWidget(this);
    layout->addWidget(widget);
    this->setLayout(layout);

    learnButton = new QPushButton("Uczenie");
    layout->addWidget(learnButton);

    scriptButton = new QPushButton("Test QProcess");
    layout->addWidget(scriptButton);


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
    learnButton->setStyleSheet(buttonStyle);

    connect(learnButton, &QPushButton::clicked, this, &neuralNetworkDialog::learnButtonClicked);
    connect(scriptButton, &QPushButton::clicked, this, &neuralNetworkDialog::runButtonClicked);

    connect(process, &QProcess::readyReadStandardOutput,this, &neuralNetworkDialog::handleProcessOutput);

    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &neuralNetworkDialog::handleScriptFinished);

}


void neuralNetworkDialog::learnButtonClicked()
{
//    torch::jit::script::Module model;
//    try {
//        model = torch::jit::load("C:/Users/Kuba/unet_nuclei_ts.pt");
//    }
//    catch (const c10::Error& e) {
//        qDebug() << "error loading the model\n";
//        qDebug() << e.what_without_backtrace();
//        return;
//    }

//    std::vector<torch::Tensor> parameters;
//    for (const auto& param : model.parameters()) {
//        parameters.push_back(param);
//    }

//    torch::optim::Adam optimizer(parameters, torch::optim::AdamOptions(0.001));
//    model.train();

//    auto custom_dataset_train = dataset().map(torch::data::transforms::Stack<>());;
//    auto data_loader_train = torch::data::make_data_loader<torch::data::samplers::RandomSampler>(std::move(custom_dataset_train), 10);


//        for (auto& batch : *data_loader_train) {
//            torch::Tensor data = batch.data;
//            torch::Tensor target = batch.target.squeeze();

//            auto sizes = data.sizes();
//            for (size_t i = 0; i < sizes.size(); ++i) {
//                std::cout << "Wymiar " << i << ": " << sizes[i] << std::endl;
//            }

//            auto sizesMask = target.sizes();
//            for (size_t i = 0; i < sizesMask.size(); ++i) {
//                std::cout << "Wymiar maska " << i << ": " << sizes[i] << std::endl;
//                }

//            optimizer.zero_grad();

//            std::vector<c10::IValue> inputs;
//            inputs.push_back(data);

//            torch::Tensor prediction;
//            try{
//                qDebug() <<"Before forward";
//                prediction = model.forward(inputs).toTensor();
//                qDebug() <<"After forward";
//            }
//            catch(const c10::Error &e){
//                qDebug() << e.what();
//            }

//            // compute loss
//            torch::Tensor loss = torch::binary_cross_entropy(prediction, target);
//            loss.backward();
//            // update weight
//            optimizer.step();
//        }

//        torch::Tensor test = imageToTensor("C:/Users/Kuba/u-net/pytorch_dataset/test/X/image_611.png");
//        std::vector<c10::IValue> testInputs;
//        testInputs.push_back(test);
//        inference(testInputs, model);
}

void neuralNetworkDialog::runButtonClicked()
{
    QString scriptPath("C:/Users/Kuba/Magisterka/test.py");
    label->setText("Uruchamianie procesu...");
    process->start("python", {scriptPath});

    if(process->state() == QProcess::Running){
        label->setText("Skrypt jest już uruchomiony!");
        return;
    }

    if(!QFile::exists(scriptPath)){
        label->setText("Błąd: Nie znaleziono pliku skryptu!");
        return;
    }
}


void neuralNetworkDialog::handleProcessOutput()
{
    QString output = process->readAllStandardOutput();
    if(!output.isEmpty()){
        label->setText(output);
    }}


void neuralNetworkDialog::handleScriptFinished(int exitCode)
{
    if(exitCode == 0){
        label->setText("Status: Sukces!");
    } else {
        label->setText("Status: Błąd!");
        label->setText(process->readAllStandardError());
    }
}
