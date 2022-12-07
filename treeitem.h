#ifndef TREEITEM_H
#define TREEITEM_H

#include <QObject>
#include <QQmlListProperty>
#include <QVariant>
#include <QVector>

class TreeItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<TreeItem> items READ items NOTIFY itemsChanged)
    Q_CLASSINFO("DefaultProperty", "items")

public:
    explicit TreeItem(QObject* parent = nullptr);
    ~TreeItem();

    void appendChildItem(TreeItem* child);

    TreeItem* parentItem();
    TreeItem* child(int row);
    QQmlListProperty<TreeItem> items();

    int childCount() const;
    int columnCount() const;
    int row() const;

    void removeChildItem(qsizetype index);
    void replaceChildItem(qsizetype index, TreeItem* newItem);
    void clear();

signals:
    void itemsChanged();

private:
    static void appendTreeItem(
        QQmlListProperty<TreeItem>* list, TreeItem* item);
    static qsizetype countTreeItems(QQmlListProperty<TreeItem>* list);
    static TreeItem* treeItem(
        QQmlListProperty<TreeItem>* list, qsizetype index);
    static void clearTreeItems(QQmlListProperty<TreeItem>* list);
    static void replaceTreeItem(
        QQmlListProperty<TreeItem>* list, qsizetype index, TreeItem* item);
    static void removeLastTreeItem(QQmlListProperty<TreeItem>* list);

private:
    TreeItem* mParentItem;
    QVector<TreeItem*> mChildItems;
};

#endif // TREEITEM_H
