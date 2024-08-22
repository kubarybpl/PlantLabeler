
#include "customproxymodel.h"

#include <QIcon>

customProxyModel::customProxyModel(QWidget *parent = nullptr) : QSortFilterProxyModel()

{}

bool customProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    // Takes filename from QModelIndex source_parent if contains "mask_" return false.
    QModelIndex baseIndex = sourceModel()->index(source_row, 0, source_parent);
    QString fileName = sourceModel()->data(baseIndex,QFileSystemModel::FileNameRole).toString();

    if(fileName.contains("mask_")){
        return 0;
    }
    return 1;
}

QVariant customProxyModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DecorationRole){
        // Casting model for processing
        auto* fileSystemModel = dynamic_cast<QFileSystemModel*>(sourceModel());

        // If file is .png or .jpg check if mask file exists. If exists provides green mask icon, otherwise black
        QModelIndex sourceIndex = mapToSource(index);
        QFileInfo fileInfo = fileSystemModel->fileInfo(sourceIndex);
        QString extension = fileInfo.suffix().toLower();
        QString name = fileInfo.fileName();
        QDir dir(fileInfo.path());

        if (extension == "png" || extension == "jpg") {
            if(dir.exists("mask_" + name))
                return QIcon(":/icons/mask.png");
            else
                return QIcon(":/icons/maskBlack.png");
        }

        // Iterates over all files in directory, if all files have a mask provides green mask icon for directory, otherwise red mask
        QDir folderDir(fileInfo.absoluteFilePath());
        if(fileInfo.isDir()){
            QDirIterator it(fileInfo.absoluteFilePath());

            while(it.hasNext()) {
                it.next();
                if(it.fileName() == "." || it.fileName() == "..")
                    continue;

                if(!it.fileName().contains("mask_")){
                    if(!folderDir.exists("mask_" + it.fileName())){
                        return QIcon(":/icons/folderRed.png");
                    }
                }
            }
            return QIcon(":/icons/folder.png");
        }
    }
    return QSortFilterProxyModel::data(index, role);
}


QString customProxyModel::getFile(const QModelIndex &index, int mode)
{
    // Casting model for processing
    auto* fileSystemModel = dynamic_cast<QFileSystemModel*>(sourceModel());

    // Mapping index from source, getting parent, counting files in parent dir,
    // returning next/previous file if exists, otherwise return empty string
    QModelIndex proxyModel = this->mapFromSource(index);
    QModelIndex proxyModelParent = proxyModel.parent();

    int rows = this->rowCount(proxyModelParent);
    int i = proxyModel.row();
    if((i < rows && !mode) || (i > 0 && mode)){
        QModelIndex nextProxyIndex;
        if(!mode)
            nextProxyIndex = this->index(i + 1, 0, proxyModelParent);
        else
            nextProxyIndex = this->index(i - 1, 0, proxyModelParent);

        QModelIndex nextSourceIndex = this->mapToSource(nextProxyIndex);

        return fileSystemModel->filePath(nextSourceIndex);
    }
    else{
        qDebug() << "koniec plików";
            return QString();
    }
}

