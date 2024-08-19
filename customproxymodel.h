
/**
 * @file customproxymodel.h
 * @brief Definition of the customProxyModel class which extends QSortFilterProxyModel for custom filtering and icon decoration.
 */

#ifndef CUSTOMPROXYMODEL_H
#define CUSTOMPROXYMODEL_H

#include <QObject>
#include <QFileSystemModel>
#include <QSortFilterProxyModel>

/**
 * @brief The customProxyModel class extend QSortFilterProxyModel in order to provide filter and icon decoration methods
 */

class customProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    /**
     * @brief Constructs a customProxyModel object.
     */
    customProxyModel(QWidget *parent);
    /**
     * @brief filterAcceptsRow Filters rows based on their filename.
     * @param source_row The row index in the source model.
     * @param source_parent The parent index in the source model.
     * @return true if the row should be included in the model, otherwise false.
     */
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

    /**
     * @brief Overrides data method of QSortFilterProxyModel in order to set custom file/dir icons.
     * @param index The QModelIndex of current file/dir.
     * @param role The role for which data is required.
     * @return data stored under the given role for the item referred to by the index.
     */
    QVariant data(const QModelIndex &index, int role) const override;

    /**
     * @brief Gets the file path of the next/previous file in the directory.
     * @param index The current file index.
     * @param mode If mode is not 0, return previous file
     * @return The path of the next/previous file if available, otherwise an empty QString.
     */
    QString getFile(const QModelIndex &index, int mode);

};

#endif // CUSTOMPROXYMODEL_H
