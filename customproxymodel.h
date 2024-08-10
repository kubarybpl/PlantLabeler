
#ifndef CUSTOMPROXYMODEL_H
#define CUSTOMPROXYMODEL_H


#include <QObject>
#include <QFileSystemModel>
#include <QSortFilterProxyModel>

class customProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    customProxyModel(QWidget *parent);
    const QModelIndex getIndex(QString path);
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    setBaseModel(QFileSystemModel *model);

private:
    QFileSystemModel *baseModel;
};

#endif // CUSTOMPROXYMODEL_H
