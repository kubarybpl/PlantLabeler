
#ifndef LEFTPANEL_H
#define LEFTPANEL_H

#include <QWidget>
#include <QTreeView>
#include <QFileSystemModel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>

class leftPanel : public QWidget
{
    Q_OBJECT
public:
    leftPanel(QWidget *parent);
    ~leftPanel();
    QTreeView *treeView;
    QFileSystemModel * model;
    QVBoxLayout *leftLayout;
    QPushButton *videoButton;
private slots:
    onVideoButton();
    onTreeViewClicked(const QModelIndex &index);
signals:
    imageSelected(QString &filePath);
};

#endif // LEFTPANEL_H
