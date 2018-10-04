#ifndef ORNODE_H
#define ORNODE_H
#include "node.h"
class OrNode : public Node
{
    virtual    void drawShape(QPainter *p,QRectF &r);
public:
    OrNode(GraphWidget *graphWidget);
    virtual QRectF boundingRect() const;
    virtual bool AllowAttach(Node *) const;
    virtual void WriteSource(QTextStream &h,QTextStream &s);
    virtual double Simulate();

    // Node interface
public:
    LOGICTYPE GetLogicType() const
    {
        return fOR;
    }
};

#endif // ANDNODE_H
