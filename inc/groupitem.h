#ifndef GROUPITEM_H
#define GROUPITEM_H
#include <QAbstractTableModel>
#include "node.h"
struct GroupItem {
    int GroupID;
    QString Name;
    virtual void WriteNodeInfo(QTextStream &s);
};

class GroupTableItem : public QAbstractTableModel
{
QList<GroupItem*>GroupList;
public:
    GroupTableItem();
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &) const ;
    void AddGroupItem(int id,const QString &name);
    const GroupItem *GetItem(int n) { return GroupList.at(n); }
Node *ActiveNode;
    // QAbstractItemModel interface
public:
    int rowCount() const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const {  Q_UNUSED(parent); return 2;}
    virtual void WriteNodeInfo(QTextStream &s);
};

#endif // GROUPITEM_H
