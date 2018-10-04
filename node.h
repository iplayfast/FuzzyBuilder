#ifndef NODE_H
#define NODE_H
enum LOGICTYPE { fIN,fOUT,fFUZZY,fAND,fOR,fPID,fTIMER,fMAP};
#include "edge.h"

#include <QGraphicsItem>
#include <QList>

#include <QTextStream>
#include <qnamespace.h>

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
QString Name;
QString INTEXT;
public:
    Node(GraphWidget *graphWidget);


    QList<Edge *> edges() const;

    enum { Type = UserType + 1 };
    int type() const { return Type; }

    void calculateForces();
    bool advance();


    QPainterPath shape() const;

    virtual void addEdge(Edge *edge);


    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *);
    // scale up the input so we can simulate more varience
    double SimulateScale() { return (IOMax - IOMin < 10) ? 1024.0 : (IOMax - IOMin < 100) ? 128.0 : 1.0; }
    virtual bool AllowAttach(Node *) const=0;
    virtual void WriteSource(QTextStream &h,QTextStream &s) =0;
    virtual double Simulate()=0;
    virtual LOGICTYPE GetLogicType() const = 0;

    virtual void setPos(qreal ax, qreal ay);
    virtual void setPos(QPointF p);

    virtual QRectF boundingRect() const;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
     QVariant headerData(int section, Qt::Orientation orientation, int role) const;
     Qt::ItemFlags flags(const QModelIndex & index) const;
private:
    virtual    void drawShape(QPainter *p,QRectF &r);
     virtual QColor getGradient(int zero_one);
    bool BeenWritten;
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

    double getIOMax() const;
    void setIOMax(double value);

    double getIOMin() const;
    void setIOMin(double value);

    double getActiveValue() const;
    void setActiveValue(double value);

    QString getName() const;
    void setName(const QString &value);

    bool getBeenWritten() const;
    void setBeenWritten(bool value);

    QString getINTEXT() const;
    void setINTEXT(const QString &value);

    bool deleteEdge(Node *node);
    void ModifyEdges(bool DeleteAll);

    virtual void setNewPos(const QPointF &value);

    QPointF getNewPos() const;
private:
    bool selected;
    double Current;

    QPointF newPos;
    GraphWidget *graph;
protected:
    QList<Edge *> edgeList;
signals:
    void editCompleted(const QString &);
};
//! [0]

#endif // NODE_H


