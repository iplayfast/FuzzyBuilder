#ifndef ANDNODE_H
#define ANDNODE_H
#include "node.h"


class AndNode : public Node
{
    double AndValue;
public:
    AndNode(GraphWidget *graphWidget);
    virtual QRectF boundingRect() const;
    virtual bool AllowAttach(Node *) const;


    virtual void WriteHeader(QTextStream &h) const;
    virtual void WriteNodeInfo(QTextStream &s) const;


    virtual void FunctionData(QString &Return, QString &Parameters, QString &FunctionReturn) const;
    virtual QString Regenerate() const;

    virtual bool UsesMax() const { return false; }
    virtual QString MinText() const;
    virtual double Simulate();
    virtual QPainterPath shape() const;
    // Node interface
public:
    LOGICTYPE GetLogicType() const
    {
        return fAND;
    }
    virtual QString gettype() const { return "AND"; }
};

#endif // ANDNODE_H
