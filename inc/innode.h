#ifndef INNODE_H
#define INNODE_H
#include "nodefactory.h"
class InNode : public Node
{
public:
    InNode(GraphWidget *graphWidget);
    virtual QRectF boundingRect() const;
    virtual bool AllowAttach(Node *) const;
    virtual void WriteHeader(QTextStream &h);
    virtual void WriteNodeInfo(QTextStream &ts);
    virtual void FunctionData(QString &Return, QString &Parameters, QString &FunctionReturn);
    virtual void WriteSourcePlainGuts(QTextStream &ts);
    virtual QString Regenerate();
    virtual double Simulate();
    virtual QPainterPath shape() const;
    virtual QString MinText() { return "Min possible Real world input value"; }
    virtual QString MaxText() { return "Max possible Real world input value"; }
    virtual int ExtraMin() { return IOMin; }
    virtual int ExtraMax() { return IOMax; }
    virtual int MaxMin() { return IOMin; }
    virtual int MaxMax(int Scale=1) { return IOMax * Scale; }
    virtual bool UsesMinScale() { return true; }
    virtual bool UsesMaxScale() { return true; }
    virtual bool UsesExtra() { return true; }
    virtual QString ExtraText() { return "Simulated Input"; }
    // Node interface
public:
    LOGICTYPE GetLogicType() const
    {
        return fIN;
    }
    virtual QString gettype() const { return "IN"; }
};

#endif // ANDNODE_H
