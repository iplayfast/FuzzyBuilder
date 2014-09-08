#ifndef NOTNODE_H
#define NOTNODE_H
#include "node.h"
class NotNode : public Node
{
public:
    NotNode(GraphWidget *graphWidget);
    virtual QRectF boundingRect() const;
    virtual bool AllowAttach(Node *) const;
    virtual void WriteHeader(QTextStream &h);
    void WriteNodeInfo(QTextStream &s);
    virtual void FunctionData(QString &Return, QString &Parameters, QString &FunctionReturn);
    virtual void WriteSourcePlainGuts(QTextStream &s);
    virtual double Simulate();
    virtual QPainterPath shape() const;
    // Node interface
    virtual bool UsesMax() { return false; }
    virtual bool UsesMin() { return false; }
public:
    LOGICTYPE GetLogicType() const
    {
        return fNOT;
    }
virtual QString gettype() const { return "NOT"; }

};

#endif // NOTNODE_H
