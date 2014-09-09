#ifndef OUTNODE_H
#define OUTNODE_H
#include "nodefactory.h"
class OutNode : public Node
{
public:
    OutNode(GraphWidget *graphWidget);
    virtual QRectF boundingRect() const;
    virtual bool AllowAttach(Node *) const;

    virtual void WriteHeader(QTextStream &h);
    virtual void FunctionData(QString &Return, QString &Parameters, QString &FunctionReturn);
    virtual void WriteSourcePlainGuts(QTextStream &ts);//plain guts
    virtual void WriteNodeInfo(QTextStream &s);
    virtual QString Regenerate();
    virtual double Simulate();
    virtual QPainterPath shape() const;
    virtual bool UsesMinScale() { return true; }
    virtual bool UsesMaxScale() { return true; }

    // Node interface
public:
    LOGICTYPE GetLogicType() const
    {
        return fOUT;
    }
    virtual QString gettype() const { return "OUT"; }
};

#endif // ANDNODE_H
