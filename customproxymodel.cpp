
#include "customproxymodel.h"

#include <QIcon>

customProxyModel::customProxyModel(QWidget *parent = nullptr) : QSortFilterProxyModel()

{
//    baseModel = new QFileSystemModel;
//    baseModel->setRootPath("E:\\One Drive\\OneDrive - Politechnika Warszawska\\Magisterka\\test");
//    this->setSourceModel(baseModel);

}

const QModelIndex customProxyModel::getIndex(QString path)
{
//    return mapFromSource(baseModel->index(path));
}

bool customProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex baseIndex = sourceModel()->index(source_row, 0, source_parent);
    QString fileName = sourceModel()->data(baseIndex,QFileSystemModel::FileNameRole).toString();

    if(fileName.contains("mask_")){
        return 0;
    }
    return 1;
}

QVariant customProxyModel::data(const QModelIndex &index, int role) const
{
//    if(role == Qt::DecorationRole){
//        QString fileName = model->data(index, QFileSystemModel::FileNameRole).toString();

//        QModelIndex baseIndex = mapToSource(index);

//        QDir dir(model->filePath(baseIndex));
//        QString maskFile = "mask_" + fileName;
//        if(dir.exists(maskFile)){
//            return QIcon(":/masks/mask.png");
//        }
//        else

//            return QIcon(":/masks/C:/Users/Kuba/Downloads/mask.png");
//    }
    //    return QSortFilterProxyModel::data(index, role);
    ////////////////////

    if(role == Qt::DecorationRole){

        auto* fileSystemModel = dynamic_cast<QFileSystemModel*>(sourceModel());

        QModelIndex sourceIndex = mapToSource(index);                   // zmapowany do żródła indeks
        QFileInfo fileInfo = fileSystemModel->fileInfo(sourceIndex);
        QString extension = fileInfo.suffix().toLower();
        QString name = fileInfo.fileName();

        if (extension == "png" || extension == "jpg") {
            QDir dir(fileInfo.path());

            if(dir.exists("mask_" + name))
                return QIcon(":/masks/C:/Users/Kuba/Downloads/mask.png");
            else
                return QIcon(":/masks/C:/Users/Kuba/Downloads/maskBlack.png");
        }
    }
    return QSortFilterProxyModel::data(index, role);
}

customProxyModel::setBaseModel(QFileSystemModel *model)
{
    this->setSourceModel(model);
}

