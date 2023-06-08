#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QQmlEngine>
#include <QQmlListProperty>
#include <QVariant>

class TreeItem;

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT
    QML_ELEMENT
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

    Q_INVOKABLE QModelIndex insert(QJsonObject data,
        const QModelIndex& parent = QModelIndex(), int pos = -1);
    Q_INVOKABLE TreeItem* itemFromIndex(const QModelIndex& index) const;
    Q_INVOKABLE void clear();

    QQmlListProperty<TreeItem> items();

private slots:
    void onTreeItemDataChanged();
    void onTreeItemChildItemAppendedPrivately(TreeItem* child);

private:
    void setupModelData(const QStringList& lines, TreeItem* parent);

    void updateRoleNamesWithTreeItemKeys(TreeItem* item);

    void appendTreeItemToRoot(TreeItem* item);
    void traverseTreeItemForNewItems(TreeItem* item);

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
