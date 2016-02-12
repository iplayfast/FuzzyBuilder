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
class Node : public QObject, public QGraphicsItem
{
    struct lmc{
        bool Used;
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
    virtual void FunctionData(QString &Return, QString &Parameters, QString &FunctionReturn, bool &HasBrackets) const;


    // scale up the input so we can simulate more varience
    double SimulateScale() { return (IOMax - IOMin < 10) ? 1024.0 : (IOMax - IOMin < 100) ? 128.0 : 1.0; }
    virtual bool AllowAttach(Node *) const=0;


    virtual void WriteHeader(QTextStream &h) const;
    virtual void WriteIncludes(QTextStream &h) const;
    virtual void WriteNodeInfo(QTextStream &s) const;
    void WriteSourceUserBefore(QTextStream &s) const; // user modified
    void WriteSourceUserGuts(QTextStream &s) const; // user modified plus plain guts
    virtual void WriteSourcePlainGuts(QTextStream &s) const;//plain guts
    virtual void WriteEdges(QTextStream &s) const;  // connections between nodes
    virtual void WriteSource(QTextStream &s);   // calls other functions but tracks if it has been written
    virtual QString Regenerate() const;


    virtual double Simulate()=0;
    virtual LOGICTYPE GetLogicType() const = 0;

    virtual void setPos(qreal ax, qreal ay);
    virtual void setPos(QPointF p);
    QIcon generateIcon();
    double OnMinValueChanged(int Value,QString &MinText);
    double OnMaxValueChanged(int Value,QString &MinText);

    virtual bool UsesMin() const { return true; }
    virtual int MinOfMin() const { return 0; } // Min value for min slider
    virtual int MaxOfMin() const { return  MinScale; }
    virtual QString MinText() const { return ""; }
    virtual bool UsesMinScale() const { return MinScale != 1.0; }
    virtual bool UsesMaxScale() const { return MaxScale != 1.0; }
    virtual bool UsesMax() const { return true; }
    virtual int MinOfMax() const { return 0; } // min value for max slider
    virtual int MaxOfMax()const { return MaxScale; } // max value for max slider

    virtual QString MaxText() const { return ""; }
    virtual bool UsesExtra() const { return false; }
    virtual int MinOfExtra() const { return IOMin; } // min value for extra slider
    virtual int MaxOfExtra() const { return IOMax; } // max value for extra slider
    virtual QString ExtraText() const { return ""; }
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
    bool CheckSetSourceBeenWritten();
    int getNodeWidth() { return 48; }
    int getNodeHeight() { return 48; }
private:    
    bool HeaderBeenWritten;
    bool SourceBeenWritten;
    LOGICTYPE fType;
    double MinScale; // normally 1.0 unless dealing with real world IO values
    double MaxScale; // normally 1.0 unless dealing with real world IO values
    double ExtraScale; // normally 1.0 unless dealing with real world IO values
    double InValue;

    double IOMin;   // pid_p
    double IOMax;   // pid_i
    double ActiveValue; // pid_d
public:
    virtual QString getIOMinText()const;
    virtual QString getIOMaxText() const;
    virtual QString getExtraText() const;

    virtual QString GetValueText() const;
    bool getSelected() const;
    void setSelected(bool value);

    double getCurrent() const;
    void setCurrent(double value);

    void setInGroup(int GroupID,bool IsIn);
    virtual double getIOMax() const;
    virtual void setIOMax(double value);

    double getIOMin() const;
    virtual void setIOMin(double value);

    double getActiveValue() const;
    void setActiveValue(double value);

    QString getName() const;
    virtual void setName(const QString &value);

    QString getInLineText() const;
    void setINTEXT(const QString &value);

    bool deleteEdge(Node *node);
    void JoinRemoveEdgesFromSelectedNodes();

    virtual void setNewPos(const QPointF &value);

    QPointF getNewPos() const;
    void DeleteAllEdges();

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
    double getMinScale() const;
    void setMinScale(double value);

    double getMaxScale() const;
    void setMaxScale(double value);

    double getExtraScale() const;
    void setExtraScale(double value);

    double getInValue() const;
    void setInValue(double value);
    double Normalize(double value) const;
    double UnNormalize(double value) const;
    virtual QString InitizationCode() const { return ""; }
    bool getHeaderBeenWritten() const;
    void setHeaderBeenWritten(bool value);

public slots:
    void OnClick();
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


