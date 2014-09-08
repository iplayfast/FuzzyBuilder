#ifndef ORNODE_H
#define ORNODE_H

#include "node.h"

class OrNode : public Node
{

public:
    OrNode(GraphWidget *graphWidget);
    virtual QRectF boundingRect() const;
    virtual bool AllowAttach(Node *) const;
    virtual void WriteHeader(QTextStream &h);
    void WriteNodeInfo(QTextStream &s);
    virtual void FunctionData(QString &Return, QString &Parameters, QString &FunctionReturn);
    virtual void WriteSourcePlainGuts(QTextStream &ts);
    virtual bool UsesMax() { return false; }
    virtual QString Regenerate();
    virtual double Simulate();
    virtual QPainterPath shape() const;
    // Node interface
public:
    LOGICTYPE GetLogicType() const
    {
        return fOR;
    }
    virtual QString gettype() const { return "OR"; }
};

#endif // ANDNODE_H
