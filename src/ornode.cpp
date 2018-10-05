#include "ornode.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <qevent.h>
#include <qnamespace.h>
#include <QDebug>

void OrNode::drawShape(QPainter *p, QRectF &r)
{
    p->drawEllipse(r);
}


OrNode::OrNode(GraphWidget *graphWidget) : Node(graphWidget)
{

}

QRectF OrNode::boundingRect() const
{
int width = 20;
int height = 20;
qreal adjust = 2;
return QRectF( -width - adjust, -height - adjust, 2 * width + adjust, 2 * height + adjust);
}

bool OrNode::AllowAttach(Node *) const
{
    return true;
}



void OrNode::WriteSource(QTextStream &h,QTextStream &s)
{
    if (getBeenWritten()) return;
    setBeenWritten(true);
    QString ss;
        ss.sprintf("%05.5f",ActiveValue);


    foreach (Edge *edge, edgeList)
    {
        if (edge->sourceNode()!=this)
            edge->sourceNode()->WriteSource(h,s);
    }
    QString ps; ps.sprintf("!!%f!!%f\n",pos().rx(),pos().ry());
    h << "//!!fOr!!" << getName() << "!!" << ss << ps;
    h << "double " << getName() << "(void);\n";
    s << "\ndouble " << getName() << "(void)\n{\n";
    s << "double  temp,Current = " << ss << ";\n";
        foreach (Edge *edge, edgeList)
        {
            if (edge->sourceNode()!=this)
            {
                s << "   temp = " << edge->sourceNode()->getName() << "();\n";
                s << "   if (Current < temp) Current = temp;\n";
            }
        }
        s << "return Current;\n}\n";
        foreach (Edge *edge, edgeList)
            if (edge->sourceNode()!=this)
                edge->WriteSource(h);

}

double OrNode::Simulate()
{
double    Current = 0.0;
    foreach (Edge *edge, edgeList)
    {
        if (edge->sourceNode()!=this)
        {
            double v = edge->sourceNode()->Simulate();
            if (Current<v) Current = v;
        }
    }
    setCurrent(Current);
    return Current;

}
