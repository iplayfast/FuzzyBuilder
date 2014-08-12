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

    virtual void WriteHeader(QTextStream &h);
    void WriteNodeInfo(QTextStream &s);


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
    virtual QString gettype() const { return "PID"; }
};

#endif // ANDNODE_H
