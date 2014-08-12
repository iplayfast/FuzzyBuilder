#ifndef FUZZYNODE_H
#define FUZZYNODE_H
#include <QAbstractTableModel>
#include "nodefactory.h"
#include "TFuzzy.h"
using namespace Crystal;
using namespace Fuzzy;

class FuzzyNode : public Node, public QAbstractTableModel
{
public:
    FuzzyNode(GraphWidget *graphWidget);
    virtual QRectF boundingRect() const;
    virtual bool AllowAttach(Node *) const;

    virtual void WriteHeader(QTextStream &h);    
    void WriteNodeInfo(QTextStream &s);
    virtual void FunctionData(QString &Return, QString &Parameters, QString &FunctionReturn);
    virtual void WriteSourcePlainGuts(QTextStream &s);//plain guts


    virtual double Simulate();
    ClassFuzzy fuzzy;
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual QPainterPath shape() const;
     Qt::ItemFlags flags(const QModelIndex &) const ;
    // Node interface
public:
    LOGICTYPE GetLogicType() const
    {
     return fFUZZY;
    }
    virtual QString gettype() const { return "FUZZY"; }    
};

#endif // ANDNODE_H
