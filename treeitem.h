#ifndef TREEITEM_H
#define TREEITEM_H

#include <QObject>
#include <QVariant>
#include <QVector>

class TreeItem : public QObject
{
public:
    explicit TreeItem(
        const QVector<QVariant>& data, TreeItem* parentItem = nullptr);
    ~TreeItem();

    void appendChild(TreeItem* child);

    TreeItem* parentItem();
    TreeItem* child(int row);

    int childCount() const;
    int columnCount() const;
    int row() const;

    QVariant data(int column) const;

private:
    TreeItem* mParentItem;
    QVector<TreeItem*> mChildItems;

    QVector<QVariant> mItemData;
};

#endif // TREEITEM_H
