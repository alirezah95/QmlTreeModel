#include "treeitem.h"

TreeItem::TreeItem(const QVector<QVariant>& data, TreeItem* parentItem)
    : QObject(parentItem), mItemData(data), mParentItem(parentItem)
{
}

TreeItem::~TreeItem()
{
    qDeleteAll(mChildItems);
    return;
}

void TreeItem::appendChild(TreeItem* item)
{
    mChildItems.append(item);
    return;
}

TreeItem* TreeItem::child(int row)
{
    if (row < 0 || row >= mChildItems.size()) {
        return nullptr;
    }

    return mChildItems.at(row);
}

int TreeItem::childCount() const
{
    return mChildItems.count();
}

int TreeItem::columnCount() const
{
    return mItemData.count();
}

QVariant TreeItem::data(int column) const
{
    if (column < 0 || column >= mItemData.size()) {
        return QVariant();
    }

    return mItemData.at(column);
}

TreeItem* TreeItem::parentItem()
{
    return mParentItem;
}

int TreeItem::row() const
{
    if (mParentItem) {
        return mParentItem->mChildItems.indexOf(const_cast<TreeItem*>(this));
    }

    return 0;
}
