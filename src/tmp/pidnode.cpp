#include "pidnode.h"
#include "graphwidget.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <qevent.h>
#include <qnamespace.h>
#include <QDebug>

PidNode::PidNode(GraphWidget *graphWidget) : Node(graphWidget),SetPoint(graphWidget),Input(graphWidget),Direction(graphWidget)
{   
    SetPoint.setINTEXT("Requested(SetPoint)");
    Input.setINTEXT("Obtained(Input)");
    Direction.setINTEXT("Direction");
    setNewPos(pos());
}

QRectF PidNode::boundingRect() const
{
int width = 20;
int height = 20;
qreal adjust = 2;
return QRectF( -width - adjust, -height - adjust, 2 * width + adjust, 2 * height + adjust);
}

bool PidNode::AllowAttach(Node *node) const
{    
    return (SetPoint.AllowAttach(node) || Input.AllowAttach(node) || Direction.AllowAttach(node));
}

void PidNode::addEdge(Edge *edge)
{
    if (SetPoint.AllowAttach(edge->sourceNode())) SetPoint.addEdge(edge);
    else {
        if (Input.AllowAttach(edge->sourceNode())) Input.addEdge(edge);
        else if (Direction.AllowAttach(edge->sourceNode())) Direction.addEdge(edge);
            else Node::addEdge(edge);// shouldn't get here
    }
}




void PidNode::WriteSource(QTextStream &h,QTextStream &s)
{
    if (getBeenWritten()) return;
    setBeenWritten(true);
    QString ps; ps.sprintf("!!%f!!%f\n",pos().rx(),pos().ry());
    h << "//!!fPid!!" << getName() << ps;
    SetPoint.WriteSource(h,s);
    Input.WriteSource(h,s);
    foreach (Edge *edge, edgeList)
        if (edge->sourceNode()!=this)
            edge->WriteSource(h);




    // todo

    s << "// here is the source of the pid controller... todo\n\n";
}

double PidNode::Simulate()
{
    // todo
    foreach (Edge *edge, edgeList)
    {
        if (edge->sourceNode()!=this)
        {
            InValue  = edge->sourceNode()->Simulate();
            //double error =

        }
    }
    return getCurrent();

}

void PidNode::setNewPos(const QPointF &value)
{
    Node::setNewPos(value);
    SetPoint.setPos(value.x()-25,value.y()-20);
    Input.setPos(value.x()-25,value.y()+20);
    Direction.setPos(value.x(),value.y()-20);
}
