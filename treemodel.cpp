#include "treemodel.h"
#include "treeitem.h"

#include <QMetaProperty>

TreeModel::TreeModel(QObject* parent) : QAbstractItemModel(parent)
{
    mRootItem = new TreeItem({});
}

TreeModel::~TreeModel()
{
    delete mRootItem;
    return;
}

int TreeModel::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid()) {
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    }

    return mRootItem->columnCount();
}

QModelIndex TreeModel::insert(
    QJsonObject data, const QModelIndex& parent, int pos)
{
    TreeItem* parentElement = itemFromIndex(parent);

    if (pos >= parentElement->childCount()) {
        return QModelIndex();
    }

    if (pos < 0) {
        pos = parentElement->childCount();
    }

    beginInsertRows(parent, pos, pos);

    TreeItem* item = new TreeItem();
    item->setData(data);

    bool res = parentElement->insertChildItem(pos, item);
    // Add keys in TreeItem's data to role names
    updateRoleNamesWithTreeItemKeys(item);

    // Connect to dataChanged() and childItemAppendedPrivately() signals
    connect(
        item, &TreeItem::dataChanged, this, &TreeModel::onTreeItemDataChanged);
    connect(item, &TreeItem::childItemAppendedPrivately, this,
        &TreeModel::onTreeItemChildItemAppendedPrivately);

    endInsertRows();

    return createIndex(pos, 1, item);
}

QQmlListProperty<TreeItem> TreeModel::items()
{
    return {
        this,
        this,
        &TreeModel::appendItem,
        &TreeModel::countItems,
        &TreeModel::item,
        &TreeModel::clearItems,
    };
}

void TreeModel::onTreeItemDataChanged()
{
    if (auto item = qobject_cast<TreeItem*>(QObject::sender())) {
        updateRoleNamesWithTreeItemKeys(item);
    }
}

void TreeModel::onTreeItemChildItemAppendedPrivately(TreeItem* child)
{
    if (TreeItem* parentItem = child->parentItem()) {
        int parentRow = parentItem->row();
        QModelIndex parentIndex = createIndex(parentRow, 1, parentItem);

        int childRow = child->row();
        beginInsertRows(parentIndex, childRow, childRow);

        updateRoleNamesWithTreeItemKeys(child);
        // Connect to dataChanged() and childItemAppendedPrivately() signals
        connect(child, &TreeItem::dataChanged, this,
            &TreeModel::onTreeItemDataChanged);
        connect(child, &TreeItem::childItemAppendedPrivately, this,
            &TreeModel::onTreeItemChildItemAppendedPrivately);

        endInsertRows();
    }
}

QVariant TreeModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (!mRoleNames.contains(role)) {
        return QVariant();
    }

    TreeItem* item = static_cast<TreeItem*>(index.internalPointer());

    QString roleName = mRoleNames.value(role);

    return item->data().value(roleName);
}

bool TreeModel::setData(
    const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid()) {
        return false;
    }

    if (!mRoleNames.contains(role)) {
        return false;
    }

    TreeItem* item = static_cast<TreeItem*>(index.internalPointer());

    QString roleName = mRoleNames.value(role);
    item->data()[roleName] = value.toJsonValue();

    emit dataChanged(index, index, { role });
    return true;
}

QModelIndex TreeModel::index(
    int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    TreeItem* parentItem;

    if (!parent.isValid()) {
        parentItem = mRootItem;
    } else {
        parentItem = static_cast<TreeItem*>(parent.internalPointer());
    }

    TreeItem* childItem = parentItem->child(row);
    if (childItem) {
        return createIndex(row, column, childItem);
    }

    return QModelIndex();
}

QHash<int, QByteArray> TreeModel::roleNames() const
{
    return mRoleNames;
}

QModelIndex TreeModel::parent(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    TreeItem* childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem* parentItem = childItem->parentItem();

    if (parentItem == mRootItem) {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex& parent) const
{
    TreeItem* parentItem;
    if (parent.column() > 0) {
        return 0;
    }

    if (!parent.isValid()) {
        parentItem = mRootItem;
    } else {
        parentItem = static_cast<TreeItem*>(parent.internalPointer());
    }

    return parentItem->childCount();
}

void TreeModel::setupModelData(const QStringList& lines, TreeItem* parent)
{
    /*
    QVector<TreeItem*> parents;
    QVector<int> indentations;
    parents << parent;
    indentations << 0;

    int number = 0;

    while (number < lines.count()) {
        int position = 0;
        while (position < lines[number].length()) {
            if (lines[number].at(position) != ' ') {
                break;
            }
            position++;
        }

        const QString lineData = lines[number].mid(position).trimmed();

        if (!lineData.isEmpty()) {
            // Read the column data from the rest of the line.
            const QStringList columnStrings
                = lineData.split(QLatin1Char('\t'), Qt::SkipEmptyParts);

            QVector<QVariant> columnData;
            columnData.reserve(columnStrings.count());
            for (const QString& columnString : columnStrings) {
                columnData << columnString;
            }

            if (position > indentations.last()) {
                if (parents.last()->childCount() > 0) {
                    parents << parents.last()->child(
                        parents.last()->childCount() - 1);
                    indentations << position;
                }
            } else {
                while (position < indentations.last() && parents.count() > 0) {
                    parents.pop_back();
                    indentations.pop_back();
                }
            }

            parents.last()->appendChild(
                new TreeItem(columnData, parents.last()));
        }
        number++;
    }
    */
}

void TreeModel::updateRoleNamesWithTreeItemKeys(TreeItem* item)
{
    // Add keys in TreeItem's data to role names
    auto& itemData = item->data();
    const auto& roles = mRoleNames.values();
    for (auto it = itemData.constBegin(); it != itemData.constEnd(); it++) {
        if (!roles.contains(it.key())) {
            mRoleNames[mLastRoleValue++] = it.key().toUtf8();
        }
    }
}

void TreeModel::appendTreeItemToRoot(TreeItem* item)
{
    int pos = mRootItem->childCount();
    beginInsertRows(QModelIndex(), pos, pos);

    mRootItem->insertChildItem(pos, item);

    // Add keys in TreeItem's data to role names
    updateRoleNamesWithTreeItemKeys(item);

    // Connect to dataChanged() and childItemAppendedPrivately() signals
    connect(
        item, &TreeItem::dataChanged, this, &TreeModel::onTreeItemDataChanged);
    connect(item, &TreeItem::childItemAppendedPrivately, this,
        &TreeModel::onTreeItemChildItemAppendedPrivately);

    if (item->childCount() > 0) {
        // All of TreeItems in 'items' property of this TreeItem should be
        // traversed to add their keys to roleNames
        traverseTreeItemForNewItems(item);
    }

    endInsertRows();
}

void TreeModel::traverseTreeItemForNewItems(TreeItem* item)
{
    for (int i = 0; i < item->childCount(); ++i) {
        TreeItem* currentItem = item->child(i);
        // Add keys in TreeItem's data to role names
        updateRoleNamesWithTreeItemKeys(currentItem);

        // Connect to dataChanged() and childItemAppendedPrivately() signals
        connect(currentItem, &TreeItem::dataChanged, this,
            &TreeModel::onTreeItemDataChanged);
        connect(currentItem, &TreeItem::childItemAppendedPrivately, this,
            &TreeModel::onTreeItemChildItemAppendedPrivately);

        if (currentItem->childCount() > 0) {
            traverseTreeItemForNewItems(currentItem);
        }
    }
}

TreeItem* TreeModel::itemFromIndex(const QModelIndex& index) const
{
    if (index.isValid()) {
        return static_cast<TreeItem*>(index.internalPointer());
    }
    return mRootItem;
}

void TreeModel::clear()
{
    beginResetModel();
    mRootItem->clear();
    mRoleNames.clear();
    mLastRoleValue = Qt::UserRole;
    endResetModel();
}

void TreeModel::appendItem(QQmlListProperty<TreeItem>* prop, TreeItem* item)
{
    reinterpret_cast<TreeModel*>(prop->object)->appendTreeItemToRoot(item);
}

qsizetype TreeModel::countItems(QQmlListProperty<TreeItem>* prop) { }

TreeItem* TreeModel::item(QQmlListProperty<TreeItem>* prop, qsizetype index) { }

void TreeModel::clearItems(QQmlListProperty<TreeItem>* prop) { }
