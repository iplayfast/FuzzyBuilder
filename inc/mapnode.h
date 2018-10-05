#ifndef MAPNODE_H
#define MAPNODE_H
#include "node.h"

class mapnode : public Node
{
public:
    mapnode(GraphWidget *graphWidget);
    virtual bool AllowAttach(Node *) const;
    virtual void WriteSource(QTextStream &h,QTextStream &s);
    virtual double Simulate();

    // Node interface
public:
    LOGICTYPE GetLogicType() const
    {
        return fMAP;
    }
};

#endif // MAPNODE_H
