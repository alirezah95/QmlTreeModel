#include "treeitem.h"

TreeItem::TreeItem(QObject* parent) : QObject(parent), mParentItem(nullptr) { }

TreeItem::~TreeItem()
{
    qDeleteAll(mChildItems);
    return;
}

void TreeItem::appendChildItem(TreeItem* item)
{
    mChildItems.append(item);
    return;
}

bool TreeItem::insertChildItem(qsizetype index, TreeItem* child)
{
    if (index < 0 || index > mChildItems.size()) {
        return false;
    }

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
        this,
        &TreeItem::appendTreeItem,
        &TreeItem::countTreeItems,
        &TreeItem::treeItem,
        &TreeItem::clearTreeItems,
        &TreeItem::replaceTreeItem,
        &TreeItem::removeLastTreeItem,
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

void TreeItem::appendTreeItem(QQmlListProperty<TreeItem>* list, TreeItem* item)
{
    reinterpret_cast<TreeItem*>(list->data)->appendChildItem(item);
    return;
}

qsizetype TreeItem::countTreeItems(QQmlListProperty<TreeItem>* list)
{
    return reinterpret_cast<TreeItem*>(list->data)->childCount();
}

TreeItem* TreeItem::treeItem(QQmlListProperty<TreeItem>* list, qsizetype index)
{
    return reinterpret_cast<TreeItem*>(list->data)->child(index);
}

void TreeItem::clearTreeItems(QQmlListProperty<TreeItem>* list)
{
    reinterpret_cast<TreeItem*>(list->data)->clear();
}

void TreeItem::replaceTreeItem(
    QQmlListProperty<TreeItem>* list, qsizetype index, TreeItem* item)
{
    reinterpret_cast<TreeItem*>(list->data)->replaceChildItem(index, item);
    return;
}

void TreeItem::removeLastTreeItem(QQmlListProperty<TreeItem>* list)
{
    auto item = reinterpret_cast<TreeItem*>(list->data);
    item->removeChildItem(item->childCount() - 1);
    return;
}
