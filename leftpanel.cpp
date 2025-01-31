
#include "leftpanel.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QDebug>
#include <QMessageBox>
#include <filesystem>
#include <customproxymodel.h>

// ROOT_PATH is target directory for saved frames
// FRAME_INTERVAL defines number of frames between every saved image
#define ROOT_PATH               "E://One Drive//OneDrive - Politechnika Warszawska//Magisterka//test"
//#define ROOT_PATH               "E://One Drive//OneDrive - Politechnika Warszawska//Magisterka//nuclei"
#define FRAME_INTERVAL          10 // Take n-th frame from video to be processed

using namespace cv;

leftPanel::leftPanel(QWidget *parent) : QWidget(parent)
{
    leftLayout = new QVBoxLayout(this);
    videoButton = new QPushButton("Dodaj");

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

    videoButton->setStyleSheet(buttonStyle);

    videoButton->setFixedSize(100,30);
    setMinimumWidth(300);
    setMaximumWidth(450);

    leftLayout->addWidget(videoButton);

    model = new QFileSystemModel;
    model->setRootPath(ROOT_PATH);

    treeView = new QTreeView(this);
    proxyModel = new customProxyModel(this);
    proxyModel->setSourceModel(model);

    treeView->setModel(proxyModel);
    treeView->setRootIndex(proxyModel->mapFromSource(model->index(ROOT_PATH)));

    // Show only filename column
    treeView->hideColumn(1);
    treeView->hideColumn(2);
    treeView->hideColumn(3);
    treeView->resize(300,150);

    leftLayout->addWidget(treeView);


    connect(videoButton, &QPushButton::clicked, this, &leftPanel::onVideoButton);
    connect(treeView, &QTreeView::clicked, this, &leftPanel::onTreeViewClicked);
}

leftPanel::~leftPanel()
{

}

QString leftPanel::getNextFileFromTree(QString path, int mode)
{
    QModelIndex index = model->index(path);
    return proxyModel->getFile(index, mode);
}

void leftPanel::selectModelInTree(QString path)
{
    QModelIndex index = model->index(path);
    treeView->setCurrentIndex(proxyModel->mapFromSource(index));
    treeView->selectionModel()->select(proxyModel->mapFromSource(index), QItemSelectionModel::Select | QItemSelectionModel::Rows);
}

void leftPanel::onTreeViewClicked(const QModelIndex &index)
{
    QModelIndex sourceIndex = proxyModel->mapToSource(index);
    QString filePath = model->filePath(sourceIndex);
    emit imageSelected(filePath);
}

void leftPanel::onVideoButton()
{
    QString videoName = QFileDialog::getOpenFileName(this, "Wybierz plik wideo","E:/One Drive/OneDrive - Politechnika Warszawska/Magisterka/09.2023","AVI(*.avi)");

    if (!videoName.isEmpty()) {
        // Make new directory with name of chosen video file
        QFileInfo fileInfo(videoName);
        QString folderName = fileInfo.baseName();
        QString targetFolder = ROOT_PATH;
        QString newFolderDir = targetFolder + "/" + folderName;

        // If directory already exists - break
        for (const auto &entry : std::filesystem::directory_iterator(targetFolder.toStdString()))
            if(entry.path() == std::filesystem::path(newFolderDir.toStdString())){
                QMessageBox::warning(this, "Błąd", "Taki Folder już istnieje");
                return;
            }

        QDir().mkdir(newFolderDir);

        // Load video and save frames with interval defined by FRAME_INTERVAL
        cv::VideoCapture video(videoName.toStdString());
        int frameCount = 0;
        cv::Mat frame;
        while(video.isOpened()){
            video >> frame;

            if(frame.empty()){
                qDebug("frame is empty");
                break;
            }

            if(frameCount % FRAME_INTERVAL == 0){
                QString imagePath = newFolderDir + "/" + folderName.last(19) + "_frame" + QString::number(frameCount) + ".png";
                cv::imwrite(imagePath.toStdString(), frame);
            }

            frameCount++;
        }
        video.release();
    }
}

