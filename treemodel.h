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
    bool setData(
        const QModelIndex& index, const QVariant& value, int role) override;
    QModelIndex parent(const QModelIndex& index) const override;
    QModelIndex index(int row, int column,
        const QModelIndex& parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    Q_INVOKABLE bool insert(TreeItem* item,
        const QModelIndex& parent = QModelIndex(), int pos = -1);

private:
    void setupModelData(const QStringList& lines, TreeItem* parent);

    Q_INVOKABLE TreeItem* elementFromIndex(const QModelIndex& index) const;

private:
    TreeItem* mRootItem = nullptr;

    int mLastRoleValue = Qt::UserRole;

    QHash<int, QByteArray> mRoleNames;
};

#endif // TREEMODEL_H
