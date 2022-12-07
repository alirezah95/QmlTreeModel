#include "treeitem.h"

TreeItem::TreeItem(QObject* parent) : QObject(parent), mParentItem(nullptr) { }

TreeItem::~TreeItem()
{
    qDeleteAll(mChildItems);
    return;
}

void TreeItem::appendChildItem(TreeItem* item)
{
    item->mParentItem = this;
    item->setParent(this);

    mChildItems.append(item);
    return;
}

bool TreeItem::insertChildItem(qsizetype index, TreeItem* child)
{
    if (index < 0 || index > mChildItems.size()) {
        return false;
    }

    child->mParentItem = this;
    child->setParent(this);

    mChildItems.insert(index, child);
    return true;
}

TreeItem* TreeItem::child(int row)
{
    if (row < 0 || row >= mChildItems.size()) {
        return nullptr;
    }

    return mChildItems.at(row);
}

QQmlListProperty<TreeItem> TreeItem::items()
{
    return {
        this,
        &mData,
        &TreeItem::appendChildItem,
        &TreeItem::countTreeItems,
        &TreeItem::treeItem,
        &TreeItem::clearChildItems,
    };
}

int TreeItem::childCount() const
{
    return mChildItems.count();
}

int TreeItem::columnCount() const
{
    return 1;
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

void TreeItem::removeChildItem(qsizetype index)
{
    if (index < 0 || index >= mChildItems.count()) {
        return;
    }

    mChildItems.remove(index);
    return;
}

void TreeItem::replaceChildItem(qsizetype index, TreeItem* newItem)
{
    if (index < 0 || index >= mChildItems.count()) {
        return;
    }

    mChildItems.replace(index, newItem);
    return;
}

void TreeItem::clear()
{
    return mChildItems.clear();
}

void TreeItem::setData(const QJsonObject& newData)
{
    if (mData == newData)
        return;
    mData = newData;
}

void TreeItem::appendChildItemPrivate(TreeItem* item)
{
    appendChildItem(item);
    emit childItemAppendedPrivately(item, QPrivateSignal());

    return;
}

void TreeItem::appendChildItem(QQmlListProperty<TreeItem>* list, TreeItem* item)
{
    reinterpret_cast<TreeItem*>(list->object)->appendChildItemPrivate(item);
    return;
}

qsizetype TreeItem::countTreeItems(QQmlListProperty<TreeItem>* list)
{
    return reinterpret_cast<TreeItem*>(list->object)->childCount();
}

TreeItem* TreeItem::treeItem(QQmlListProperty<TreeItem>* list, qsizetype index)
{
    return reinterpret_cast<TreeItem*>(list->object)->child(index);
}

void TreeItem::clearChildItems(QQmlListProperty<TreeItem>* list) { }
