#include "groupitem.h"

GroupTableItem::GroupTableItem()
{
    ActiveNode = 0;
}

void GroupTableItem::AddGroupItem(int id, const QString &name)
{
    foreach(GroupItem *gi,GroupList) {
        if (gi->Name==name) return; // already present
    }

    GroupItem *gi = new GroupItem();
        gi->GroupID = id;
        gi->Name = name;
        GroupList.append(gi);
        QModelIndex left = createIndex(rowCount(),0);
        QModelIndex right = createIndex(rowCount(),1);

        this->dataChanged(left,right);
}

int GroupTableItem::rowCount() const
{
       return GroupList.count();
}

int GroupTableItem::rowCount(const QModelIndex &parent) const
{
     Q_UNUSED(parent);
    return rowCount();
}

void GroupTableItem::WriteNodeInfo(QTextStream &s)
{

    s << "\n\n//\n//  A list of groupings to help sort out what is displayed\n//\n";
    foreach(GroupItem *gi,GroupList)
            gi->WriteNodeInfo(s);
    s << "\n\n";
}




QVariant GroupTableItem::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();
    GroupItem *gi = GroupList.at(row);
    switch(role) {
    case Qt::DisplayRole:
        {
            if (col==0) {
                if (ActiveNode==0) break;
                return ActiveNode->GroupMember(gi->GroupID);

            }
            else return gi->Name;
        }
        break;
    case Qt::FontRole:
        break;
    case Qt::BackgroundColorRole:
        // QBrush bg(Qt::red);
        // return bg;
        break;
    /*case Qt::TextAlignmentRole:

             if (row == 1 && col == 1) //change text alignment only for cell(1,1)
             {
                 return Qt::AlignRight + Qt::AlignVCenter;
             }
             break;*/
    case Qt::CheckStateRole:
        if (col==0) {
            if (ActiveNode==0) return Qt::PartiallyChecked;
            return ActiveNode->GroupMember(gi->GroupID) ? Qt::Checked : Qt::Unchecked;

        }
        break;

    }
    return QVariant();
}

bool GroupTableItem::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole)
       {
        int row = index.row();
        int col = index.column();
        QString s = value.toString();
        GroupItem *gi = GroupList.at(row);
        if (col==0)
            return false;
            //gi->GroupID = value.toInt();
        else
            gi->Name = value.toString();
        return true;
       }
    if (role == Qt::CheckStateRole)
    {
        int row = index.row();
        //int col = index.column();
        bool b = value.toBool();
        GroupItem *gi = GroupList.at(row);
        this->ActiveNode->setInGroup(gi->GroupID,b);
        return (index.column()==0);
    }
    return true;
}

QVariant GroupTableItem::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal) {
            switch (section)
            {
            case 0:
                return QString("Member");
            case 1:
                return QString("Group Name");
            }
        }
    }
    return QVariant();
}
Qt::ItemFlags GroupTableItem::flags(const QModelIndex &index) const
{
    if (index.column()==0)
        return Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled ;
   return Qt::ItemIsSelectable |  Qt::ItemIsEditable | Qt::ItemIsEnabled ;
}


void GroupItem::WriteNodeInfo(QTextStream &s)
{
    s << "//!!Group!!" <<  Name << "!!" << GroupID << "\n";
}
