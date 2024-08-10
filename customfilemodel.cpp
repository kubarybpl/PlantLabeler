
#include "customfilemodel.h"

customFileModel::customFileModel(QWidget *parent = nullptr) : QFileSystemModel()
{
    this->setRootPath("E:\\One Drive\\OneDrive - Politechnika Warszawska\\Magisterka\\test");
    this->setNameFilters(QStringList() << "mask_*");
}

