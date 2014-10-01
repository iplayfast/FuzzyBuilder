#ifndef PIDNODE_H
#define PIDNODE_H
#include "node.h"
#include "boundnode.h"
class PidNode : public Node
{
    BoundNode SetPoint;
    BoundNode Input;
    BoundNode Direction;
public:
    PidNode(GraphWidget *graphWidget);    
    virtual QRectF boundingRect() const;
    virtual bool AllowAttach(Node *node) const;
    virtual void addEdge(Edge *edge);

    virtual void WriteHeader(QTextStream &h) const;
    void WriteNodeInfo(QTextStream &s) const;


    virtual double Simulate();
    double get_p() const { return getIOMax(); }
    double get_i() const { return getIOMin(); }
    double get_d() const { return getActiveValue(); }
    void set_p(double v) { setIOMax(v); }
    void set_i(double v) { setIOMin(v); }
    void set_d(double v) { setActiveValue(v); }
    virtual void setNewPos(const QPointF &value);
    // Node interface
public:
    LOGICTYPE GetLogicType() const
    {
        return fPID;
    }
    virtual QString gettype() const { return "PID"; }
};

#endif // ANDNODE_H
