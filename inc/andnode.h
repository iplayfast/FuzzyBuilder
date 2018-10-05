#ifndef ANDNODE_H
#define ANDNODE_H
#include "node.h"


class AndNode : public Node
{
    double AndValue;
public:
    AndNode(GraphWidget *graphWidget);
    virtual QRectF boundingRect() const;
    virtual bool AllowAttach(Node *) const;
    virtual void WriteSource(QTextStream &h,QTextStream &s);
    virtual double Simulate();

    // Node interface
public:
    LOGICTYPE GetLogicType() const
    {
        return fAND;
    }
};

#endif // ANDNODE_H
