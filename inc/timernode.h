#ifndef TIMERNODE_H
#define TIMERNODE_H

#include "node.h"

class TimerNode : public Node
{
    virtual    void drawShape(QPainter *p,QRectF &r);
public:
    TimerNode(GraphWidget *graphWidget);
    virtual bool AllowAttach(Node *) const;
    virtual void WriteSource(QTextStream &h,QTextStream &s);
    virtual double Simulate();
    // Node interface
public:
    LOGICTYPE GetLogicType() const
    {
        return fTIMER;
    }
    virtual QString gettype() const { return "TIMER"; }
};

#endif // TIMERNODE_H
