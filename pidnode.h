#ifndef PIDNODE_H
#define PIDNODE_H
#include "node.h"
#include "boundnode.h"
class PidNode : public Node
{
    BoundNode Requested;
    BoundNode Obtained;
    virtual    void drawShape(QPainter *p,QRectF &r);
public:
    PidNode(GraphWidget *graphWidget);
    virtual QRectF boundingRect() const;
    virtual bool AllowAttach(Node *node) const;
    virtual void addEdge(Edge *edge);
    virtual void WriteSource(QTextStream &h, QTextStream &s);
    virtual double Simulate();
    double get_p() const { return IOMax; }
    double get_i() const { return IOMin; }
    double get_d() const { return ActiveValue; }
    void set_p(double v) { IOMax = v; }
    void set_i(double v) { IOMin = v; }
    void set_d(double v) { ActiveValue = v; }
    virtual void setNewPos(const QPointF &value);
    // Node interface
public:
    LOGICTYPE GetLogicType() const
    {
        return fPID;
    }
};

#endif // ANDNODE_H
