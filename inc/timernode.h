#ifndef TIMERNODE_H
#define TIMERNODE_H
#include "nodefactory.h"
class TimerNode : public Node
{
public:
    TimerNode(GraphWidget *graphWidget);
    virtual QRectF boundingRect() const;
    virtual bool AllowAttach(Node *) const;
    virtual void WriteHeader(QTextStream &h);
    void WriteNodeInfo(QTextStream &s);
virtual void FunctionData(QString &Return,QString &Parameters,QString &FunctionReturn);

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget */*widget*/);
virtual void setName(const QString &value);
    virtual double Simulate() { return 0;} // setup doesn't do any simulations
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
