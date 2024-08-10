
#ifndef LEFTPANEL_H
#define LEFTPANEL_H

#include <QWidget>
#include <QTreeView>
#include <QFileSystemModel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>
#include <customproxymodel.h>

class leftPanel : public QWidget
{
    Q_OBJECT
public:
    leftPanel(QWidget *parent);
    ~leftPanel();

private slots:
    onVideoButton();
    onTreeViewClicked(const QModelIndex &index);

signals:
    imageSelected(QString &filePath);

private:
    QTreeView *treeView;
    QVBoxLayout *leftLayout;
    QPushButton *videoButton;
    QFileSystemModel *model;
    customProxyModel *proxyModel;

};

#endif // LEFTPANEL_H
