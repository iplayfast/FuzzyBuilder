#ifndef INNODE_H
#define INNODE_H
#include "nodefactory.h"
class InNode : public Node
{
public:
    InNode(GraphWidget *graphWidget);
    virtual QRectF boundingRect() const;
    virtual bool AllowAttach(Node *n) const;
    virtual void WriteHeader(QTextStream &h) const;
    virtual void WriteNodeInfo(QTextStream &ts) const;
    virtual void FunctionData(QString &Return, QString &Parameters, QString &FunctionReturn, bool &HasBrackets) const;
    virtual void WriteSourcePlainGuts(QTextStream &ts) const;
    virtual QString Regenerate() const;
    virtual QString InitizationCode() const;
    virtual double Simulate();
    virtual QPainterPath shape() const;
    virtual QString MinText()const  { return "Min possible Real world input value"; }
    virtual QString MaxText()const  { return "Max possible Real world input value"; }
    virtual int MinOfExtra()const  { return getIOMin(); }
    virtual int MaxOfExtra()const  { return getIOMax(); }
    virtual int MaxOfMin()const;
    virtual int MaxOfMax()const ;
    virtual bool UsesMinScale() const { return true; }
    virtual bool UsesMaxScale() const { return true; }
    virtual bool UsesExtra() const { return true; }
    virtual QString ExtraText() const;
    virtual void setIOMin(double value);
    virtual void setIOMax(double value);
    virtual QString GetValueText() const;
    // Node interface
public:
    LOGICTYPE GetLogicType() const
    {
        return fIN;
    }
    virtual QString gettype() const { return "IN"; }
};

#endif // ANDNODE_H
