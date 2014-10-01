#ifndef BOUNDNODE_H
#define BOUNDNODE_H
#include "node.h"

class BoundNode : public Node
{
public:
    BoundNode(GraphWidget *graphWidget);
    virtual bool AllowAttach(Node *) const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *);
    virtual QRectF boundingRect() const;
    virtual void WriteHeader(QTextStream &h) const;


    virtual void WriteSourcePlainGuts(QTextStream &s) const;

    virtual double Simulate() {return 0;};
    virtual LOGICTYPE GetLogicType() const { return fPID; }
    virtual QString gettype() const { return ""; }
};

#endif // BOUNDNODE_H
