#ifndef INNODE_H

#define INNODE_H

#include "nodefactory.h"
class InNode : public Node
{
    virtual    void drawShape(QPainter *p,QRectF &r);
public:
    InNode(GraphWidget *graphWidget);
    virtual QRectF boundingRect() const;
    virtual bool AllowAttach(Node *) const;
    virtual void WriteSource(QTextStream &h,QTextStream &s);
    virtual double Simulate();

    // Node interface
public:
    LOGICTYPE GetLogicType() const
    {
        return fIN;
    }
};

#endif //INNODE_H
