
/**
 * @file leftpanel.h
 * @brief Definition of the leftPanel class which extends QWidget class for providing tree view and video selection
 */

#ifndef LEFTPANEL_H
#define LEFTPANEL_H

#include <QWidget>
#include <QTreeView>
#include <QFileSystemModel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>
#include <customproxymodel.h>

/**
 * @class leftPanel
 * @brief The leftPanel class provides a user interface panel containing a tree view to navigate through file systems and a button to load videos.
 */

class leftPanel : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Constructs a leftPanel widget.
     */
    leftPanel(QWidget *parent);

    /**
     * @brief Destructor of a leftPanel widget.
     */
    ~leftPanel();

    /**
     * @brief Calls method from proxy model to obtain path to the next file in tree.
     * @param path Path of current file.
     * @param mode if not 0 takes previous file path
     * @return QString to another file in directory.
     */
    QString getNextFileFromTree(QString path, int mode = 0);

    /**
     * @brief selects model in tree based on path get nextFileFromTree(QString path).
     * @param path
     * @see getNextFileFromTree(QString path);
     */
    selectModelInTree(QString path);

private slots:
    /**
     * @brief Slot triggered when the video button is clicked.
     */
    onVideoButton();

    /**
     * @brief Slot triggered when an item in the tree view is clicked.
     * @param index The model index of the clicked item.
     */
    onTreeViewClicked(const QModelIndex &index);

signals:
    /**
     * @brief Signal emitted when an image file is selected from the tree.
     * @param filePath The path of the selected image file.
     * @see onTreeViewClicked(const QModelIndex &index)
     */
    imageSelected(QString &filePath);

private:
    QTreeView *treeView;           ///< Tree view widget to display the filesystem.
    QVBoxLayout *leftLayout;       ///< Layout to arrange widgets.
    QPushButton *videoButton;      ///< Button to trigger video file selection.
    QFileSystemModel *model;       ///< Model to interact with the file system.
    customProxyModel *proxyModel;  ///< Custom proxy model for filtering and decorating the file system view.
};

#endif // LEFTPANEL_H
