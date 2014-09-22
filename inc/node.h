#ifndef NODE_H
#define NODE_H

#include "edge.h"
#include "Util.h"
#include "logictype.h"
#include <QGraphicsItem>
#include <QList>

#include <QTextStream>
#include <qnamespace.h>
#include <QPainterPath>
#include <QIcon>
#include "graphwidget.h"
class Edge;
class GraphWidget;
QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE
// internal max storage of a node
#define NODEHIGHVAL 1024.0



//! [0]
class Node : public QGraphicsItem
{
    struct lmc{
        double Least;
        double Most;
        double Current;
    };
QString Name;
QString UserGuts;
QString Notes;
int width;
int height;
public:
    Node(GraphWidget *graphWidget);


    QList<Edge *> edges() const;

    enum { Type = UserType + 1 };
    int type() const { return Type; }

    void calculateForces();
    bool advance();
    void setVisible(bool visible);
    bool GroupMember(int GroupID);
    virtual QPainterPath shape() const;

    virtual void addEdge(Edge *edge);
    virtual void FunctionData(QString &Return,QString &Parameters,QString &FunctionReturn);


    // scale up the input so we can simulate more varience
    double SimulateScale() { return (IOMax - IOMin < 10) ? 1024.0 : (IOMax - IOMin < 100) ? 128.0 : 1.0; }
    virtual bool AllowAttach(Node *) const=0;


    virtual void WriteHeader(QTextStream &h);

    virtual void WriteNodeInfo(QTextStream &s);
    void WriteSourceUserBefore(QTextStream &s); // user modified
    void WriteSourceUserGuts(QTextStream &s); // user modified plus plain guts
    virtual void WriteSourcePlainGuts(QTextStream &s);//plain guts
    virtual QString Regenerate();


    virtual double Simulate()=0;
    virtual LOGICTYPE GetLogicType() const = 0;

    virtual void setPos(qreal ax, qreal ay);
    virtual void setPos(QPointF p);
    QIcon generateIcon();
    double OnMinValueChanged(int Value,QString &MinText);
    double OnMaxValueChanged(int Value,QString &MinText);

    virtual bool UsesMin() { return true; }
    virtual int MinMin() { return 0; } // Min value for min slider
    virtual int MinMax(int Scale=1) { return NODEHIGHVAL * Scale; }
    virtual QString MinText() { return ""; }
    virtual bool UsesMinScale() { return false; }
    virtual bool UsesMaxScale() { return false; }
    virtual bool UsesMax() { return true; }
    virtual int MaxMin() { return 0; } // min value for max slider
    virtual int MaxMax(int scale=1) { return NODEHIGHVAL * scale; } // max value for max slider

    virtual QString MaxText() { return ""; }
    virtual bool UsesExtra() { return false; }
    virtual int ExtraMin() { return 0; } // min value for extra slider
    virtual int ExtraMax() { return NODEHIGHVAL; } // max value for extra slider
    virtual QString ExtraText() { return ""; }
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

     QVariant headerData(int section, Qt::Orientation orientation, int role) const;
     Qt::ItemFlags flags(const QModelIndex & index) const;
     QRectF paintSetup(QPainter *painter, const QStyleOptionGraphicsItem *option);
     virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget */*widget*/);
     virtual QString gettype() const=0;
private:
    bool HeaderBeenWritten;
    bool SourceBeenWritten;
    LOGICTYPE fType;
public:

    double InValue;

    double IOMin;   // pid_p
    double IOMax;   // pid_i
    double ActiveValue; // pid_d

    bool getSelected() const;
    void setSelected(bool value);

    double getCurrent() const;
    void setCurrent(double value);

    void setInGroup(int GroupID,bool IsIn);
    double getIOMax() const;
    virtual void setIOMax(double value);

    double getIOMin() const;
    virtual void setIOMin(double value);

    double getActiveValue() const;
    void setActiveValue(double value);

    QString getName() const;
    virtual void setName(const QString &value);

    bool getBeenWritten() const;
    void setBeenWritten(bool value);

    QString getInLineText() const;
    void setINTEXT(const QString &value);

    bool deleteEdge(Node *node);
    void JoinRemoveEdgesFromSelectedNodes();

    virtual void setNewPos(const QPointF &value);

    QPointF getNewPos() const;
    void DeleteAllEdges();
    bool getHeaderBeenWritten() const;
    void setHeaderBeenWritten(bool value);

    bool getSourceBeenWritten() const;
    void setSourceBeenWritten(bool value);

    QString getUserGuts() const;
    void setUserGuts(const QString &value);


    QString getNotes() const;
    void setNotes(const QString &value);

    int getWidth() const;
    void setWidth(int value);

    int getHeight() const;
    void setHeight(int value);

    bool FindNewVertPosition(int updown);
private:
    bool selected;
    double Current;

    QPointF newPos;
    GraphWidget *graph;

protected:
    QList<Edge *> edgeList;
    QList<int> GroupIDList; // list of groups this node is a member of

};
//! [0]

#endif // NODE_H


