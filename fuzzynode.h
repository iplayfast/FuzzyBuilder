#ifndef FUZZYNODE_H
#define FUZZYNODE_H
#include <QAbstractTableModel>
#include "nodefactory.h"
#include "TFuzzy.h"
using namespace Crystal;
using namespace Fuzzy;

class FuzzyNode : public Node, public QAbstractTableModel
{
    virtual    void drawShape(QPainter *p,QRectF &r);
public:
    FuzzyNode(GraphWidget *graphWidget);
    virtual QRectF boundingRect() const;
    virtual bool AllowAttach(Node *) const;
    virtual void WriteSource(QTextStream &h,QTextStream &s);
    virtual double Simulate();
    ClassFuzzy fuzzy;
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

    // Node interface
public:
    LOGICTYPE GetLogicType() const
    {
     return fFUZZY;
    }
};

#endif // ANDNODE_H
