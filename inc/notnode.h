#ifndef NOTNODE_H
#define NOTNODE_H
#include "node.h"
class NotNode : public Node
{
public:
    NotNode(GraphWidget *graphWidget);
    virtual QRectF boundingRect() const;
    virtual bool AllowAttach(Node *) const;
    virtual void WriteHeader(QTextStream &h) const;
    void WriteNodeInfo(QTextStream &s) const;
    virtual void FunctionData(QString &Return, QString &Parameters, QString &FunctionReturn) const;
    virtual void WriteSourcePlainGuts(QTextStream &s) const;
    virtual double Simulate();
    virtual QPainterPath shape() const;
    // Node interface
    virtual bool UsesMax() const { return false; }
    virtual bool UsesMin() const { return false; }
public:
    LOGICTYPE GetLogicType() const
    {
        return fNOT;
    }
virtual QString gettype() const { return "NOT"; }

};

#endif // NOTNODE_H
