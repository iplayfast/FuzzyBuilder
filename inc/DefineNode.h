#include "node.h"
#ifndef DEFINENODE_H
#define DEFINENODE_H
class DefineNode : public Node
{
public:
    DefineNode(GraphWidget *graphWidget);
//virtual bool UsesMinScale() const { return true; }
virtual bool UsesMax() const { return false; }
virtual QString MinText() const { return "Constant Value"; }
virtual QRectF boundingRect() const;
virtual QPainterPath shape() const;
        virtual double Simulate() { return getIOMin(); }
    virtual bool AllowAttach(Node *) const { return false; }
    virtual QString Regenerate() const;
    virtual int MaxOfMin()const { return 1023; }
    virtual void WriteNodeInfo(QTextStream &ts) const;
    virtual void FunctionData(QString &Return, QString &Parameters, QString &FunctionReturn, bool &HasBrackets) const;
    virtual void WriteSourcePlainGuts(QTextStream &ts) const;
    LOGICTYPE GetLogicType() const
    {
        return fDEFINE;
    }
    virtual QString gettype() const { return "def"; }
    virtual QString getIOMinText() const;
};

#endif // DEFINENODE_H
