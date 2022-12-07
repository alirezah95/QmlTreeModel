#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QQmlListProperty>
#include <QVariant>

class TreeItem;

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<TreeItem> items READ items)
    Q_CLASSINFO("DefaultProperty", "items")

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

    QQmlListProperty<TreeItem> items();

private:
    void setupModelData(const QStringList& lines, TreeItem* parent);

    Q_INVOKABLE TreeItem* elementFromIndex(const QModelIndex& index) const;

private:
    static void appendItem(QQmlListProperty<TreeItem>* prop, TreeItem* item);
    static qsizetype countItems(QQmlListProperty<TreeItem>* prop);
    static TreeItem* item(QQmlListProperty<TreeItem>* prop, qsizetype index);
    static void clearItems(QQmlListProperty<TreeItem>* prop);

private:
    TreeItem* mRootItem = nullptr;

    int mLastRoleValue = Qt::UserRole;

    QHash<int, QByteArray> mRoleNames;
};

#endif // TREEMODEL_H
