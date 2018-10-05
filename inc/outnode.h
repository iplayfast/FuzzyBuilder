#ifndef OUTNODE_H

#define OUTNODE_H

#include "nodefactory.h"
class OutNode : public Node
{
public:
    OutNode(GraphWidget *graphWidget);
    virtual QRectF boundingRect() const;
    virtual bool AllowAttach(Node *) const;
    virtual void WriteSource(QTextStream &h,QTextStream &s);
    virtual double Simulate();

    // Node interface
public:
    LOGICTYPE GetLogicType() const
    {
        return fOUT;
    }
private:
    void drawShape(QPainter *p, QRectF &r);
    virtual QColor getGradient(int zero_one);
};

#endif // ANDNODE_H
