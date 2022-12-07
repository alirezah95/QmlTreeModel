#ifndef TREEITEM_H
#define TREEITEM_H

#include <QJsonObject>
#include <QObject>
#include <QQmlListProperty>
#include <QVariant>
#include <QVector>

class TreeItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QJsonObject data READ data WRITE setData REQUIRED)
    Q_PROPERTY(QQmlListProperty<TreeItem> items READ items CONSTANT)
    Q_CLASSINFO("DefaultProperty", "items")

public:
    explicit TreeItem(QObject* parent = nullptr);
    ~TreeItem();

    void appendChildItem(TreeItem* child);
    bool insertChildItem(qsizetype index, TreeItem* child);

    TreeItem* parentItem();
    TreeItem* child(int row);
    QQmlListProperty<TreeItem> items();

    int childCount() const;
    int columnCount() const;
    int row() const;

    void removeChildItem(qsizetype index);
    void replaceChildItem(qsizetype index, TreeItem* newItem);
    void clear();

    QJsonObject& data() { return mData; }
    void setData(const QJsonObject& newData);

signals:
    void dataChanged(QPrivateSignal);
    void childItemAppendedPrivately(TreeItem* child, QPrivateSignal);

private:
    void appendChildItemPrivate(TreeItem* item);

    static void appendChildItem(
        QQmlListProperty<TreeItem>* list, TreeItem* item);
    static qsizetype countTreeItems(QQmlListProperty<TreeItem>* list);
    static TreeItem* treeItem(
        QQmlListProperty<TreeItem>* list, qsizetype index);
    static void clearChildItems(QQmlListProperty<TreeItem>* list);

private:
    TreeItem* mParentItem;
    QVector<TreeItem*> mChildItems;
    QJsonObject mData;
};

#endif // TREEITEM_H
