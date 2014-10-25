#ifndef ORNODE_H
#define ORNODE_H

#include "node.h"

class OrNode : public Node
{

public:
    OrNode(GraphWidget *graphWidget);
    virtual QRectF boundingRect() const;
    virtual bool AllowAttach(Node *) const;
    virtual void WriteHeader(QTextStream &h) const;
    void WriteNodeInfo(QTextStream &s) const;
    virtual QString MinText() const;
    virtual void FunctionData(QString &Return, QString &Parameters, QString &FunctionReturn, bool &HasBrackets) const;
    virtual void WriteSourcePlainGuts(QTextStream &ts) const;
    virtual bool UsesMax() const { return false; }
    virtual QString Regenerate() const;
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
