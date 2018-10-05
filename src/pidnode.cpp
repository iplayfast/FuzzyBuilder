#include "pidnode.h"
#include "graphwidget.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <qevent.h>
#include <qnamespace.h>
#include <QDebug>

void PidNode::drawShape(QPainter *p, QRectF &r)
{
    p->drawRect( r);//-5, -10, 20, 20);
}

PidNode::PidNode(GraphWidget *graphWidget) : Node(graphWidget),Requested(graphWidget),Obtained(graphWidget)
{

    QGraphicsScene *scene = graphWidget->scene();
    Requested.setINTEXT("Requested");
    Obtained.setINTEXT("Obtained");
    scene->addItem(&Requested);
    scene->addItem(&Obtained);
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
    return (Requested.AllowAttach(node) || Obtained.AllowAttach(node));
}

void PidNode::addEdge(Edge *edge)
{
    if (Requested.AllowAttach(edge->sourceNode())) Requested.addEdge(edge);
    else {
        if (Obtained.AllowAttach(edge->sourceNode())) Obtained.addEdge(edge);
        else Node::addEdge(edge);// shouldn't get here
    }
}



void PidNode::WriteSource(QTextStream &h,QTextStream &s)
{
    if (getBeenWritten()) return;
    setBeenWritten(true);
    QString ps; ps.sprintf("!!%f!!%f\n",pos().rx(),pos().ry());
    h << "//!!fPid!!" << getName() << ps;
    Requested.WriteSource(h,s);
    Obtained.WriteSource(h,s);
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
    Requested.setPos(value.x()-25,value.y()-20);
    Obtained.setPos(value.x()-25,value.y()+20);

}
