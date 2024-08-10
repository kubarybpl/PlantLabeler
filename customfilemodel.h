
#ifndef CUSTOMFILEMODEL_H
#define CUSTOMFILEMODEL_H


#include <QObject>
#include <QFileSystemModel>


class customFileModel : public QFileSystemModel
{
    Q_OBJECT
public:
    customFileModel(QWidget *parent);
};

#endif // CUSTOMFILEMODEL_H
