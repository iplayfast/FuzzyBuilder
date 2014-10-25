#ifndef TIMERNODE_H
#define TIMERNODE_H
#include "nodefactory.h"
class TimerNode : public Node
{
public:
    TimerNode(GraphWidget *graphWidget);
    virtual QRectF boundingRect() const;
    virtual bool AllowAttach(Node *) const;
    virtual void WriteHeader(QTextStream &h) const;
    void WriteNodeInfo(QTextStream &s) const;
    virtual QString MinText()const  { return "Time between checks, millisecs"; }
    virtual bool UsesMinScale() const { return true; }
    virtual int MaxOfMin()const;
    virtual QString GetValueText() const;
    virtual bool UsesMax() const { return false; }
    virtual void FunctionData(QString &Return,QString &Parameters,QString &FunctionReturn, bool &HasBrackets) const;
    void WriteSourcePlainGuts(QTextStream &s) const;
    virtual QString Regenerate() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget */*widget*/);
    virtual void setName(const QString &value);
    virtual double Simulate() { return 0;} // timer doesn't do any simulations
    virtual QPainterPath shape() const;
    // Node interface
public:
    LOGICTYPE GetLogicType() const
    {
        return fTIMER;
    }
    virtual QString gettype() const { return "TIMER"; }
};

#endif // TIMERNODE_H
