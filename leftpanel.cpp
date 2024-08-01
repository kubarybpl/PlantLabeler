
#include "leftpanel.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QDebug>
#include <QMessageBox>
#include <filesystem>

using namespace cv;

leftPanel::leftPanel(QWidget *parent = nullptr) : QWidget(parent)
{
    leftLayout = new QVBoxLayout(this);
    videoButton = new QPushButton("Dodaj");
    videoButton->setFixedSize(100,30);
//    this->setFixedWidth(250);
    this->resize(400, this->width());
    leftLayout->addWidget(videoButton);

    treeView = new QTreeView(this);
    model = new QFileSystemModel(this);
    model->setRootPath("E:\\One Drive\\OneDrive - Politechnika Warszawska\\Magisterka\\test");
    treeView->setModel(model);
    treeView->setRootIndex(model->index("E:\\One Drive\\OneDrive - Politechnika Warszawska\\Magisterka\\test"));
    treeView->resize(300,150);

    leftLayout->addWidget(treeView);


    connect(videoButton, &QPushButton::clicked, this, &leftPanel::onVideoButton);
    connect(treeView, &QTreeView::clicked, this, &leftPanel::onTreeViewClicked);
}

leftPanel::~leftPanel()
{

}

leftPanel::onTreeViewClicked(const QModelIndex &index)
{
    QString filePath = model->filePath(index);

    emit imageSelected(filePath);

}

leftPanel::onVideoButton()
{
    QString videoName = QFileDialog::getOpenFileName(this, "Wybierz plik wideo","E:/One Drive/OneDrive - Politechnika Warszawska/Magisterka/09.2023","AVI(*.avi)");

    if (!videoName.isEmpty()) {
        qDebug("weszło w !folderPath.isEmpty()");
        QFileInfo fileInfo(videoName);
        QString folderName = fileInfo.baseName();
        QString targetFolder = "E:/One Drive/OneDrive - Politechnika Warszawska/Magisterka/test";
        QString newFolderDir = targetFolder + "/" + folderName;

        // Jeżeli jest już taki folder -> poinformuj/zrzuć
        for (const auto &entry : std::filesystem::directory_iterator(targetFolder.toStdString()))
            if(entry.path() == std::filesystem::path(newFolderDir.toStdString())){
                QMessageBox::warning(this, "Błąd", "Taki Folder już istnieje");
                return 0;
            }

        QDir().mkdir(newFolderDir);

        cv::VideoCapture video(videoName.toStdString());
        int frameCount = 0;
        cv::Mat frame;
        while(video.isOpened()){
            video >> frame;

            if(frame.empty()){
                // wyświetlić komiunikat
                qDebug("frame is empty");
                break;
            }
//            qDebug() << "Przeszło łapanie klatek";
            if(frameCount % 50 == 0){
                QString imagePath = newFolderDir + "/" + folderName.last(19) + "_frame" + QString::number(frameCount) + ".png";
                imwrite(imagePath.toStdString(), frame);
                //qDebug().nospace() << "Zapisało jako: " << imagePath.toStdString();
            }

            frameCount++;
        }
        video.release();
    }
}

