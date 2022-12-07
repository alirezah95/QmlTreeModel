#ifndef TREEMODEL_H
#define TREEMODEL_H
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

class TreeItem;

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit TreeModel(QObject* parent = nullptr);
    ~TreeModel();

    QVariant data(const QModelIndex& index, int role) const override;
    QModelIndex parent(const QModelIndex& index) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
        int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
        const QModelIndex& parent = QModelIndex()) const override;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    Q_INVOKABLE bool insert(TreeItem* item,
        const QModelIndex& parent = QModelIndex(), int pos = -1);

private:
    void setupModelData(const QStringList& lines, TreeItem* parent);

    Q_INVOKABLE TreeItem* elementFromIndex(const QModelIndex& index) const;

private:
    TreeItem* mRootItem = nullptr;
};

#endif // TREEMODEL_H
