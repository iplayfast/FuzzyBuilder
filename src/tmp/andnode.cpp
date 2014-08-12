#include "andnode.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <qevent.h>
#include <qnamespace.h>
#include <QDebug>

AndNode::AndNode(GraphWidget *graphWidget) : Node(graphWidget)
{

    AndValue = 1.0;
}

QRectF AndNode::boundingRect() const
{
int width = 20;
int height = 20;
qreal adjust = 2;
return QRectF( -width - adjust, -height - adjust, 2 * width + adjust, 2 * height + adjust);
}

bool AndNode::AllowAttach(Node *) const
{
    return true;
}




void AndNode::WriteSource(QTextStream &h,QTextStream &s)
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
    h << "//!!fAnd!!" << getName() << "!!" << ss << ps;
    h << "\ndouble " << getName() << "(void);\n";
    s << "\ndouble " << getName() << "(void)\n{\n";
    s << "double  temp,Current = " << ss << ";\n";
        foreach (Edge *edge, edgeList)
        {
            if (edge->sourceNode()!=this)
            {
                s << "   temp = " << edge->sourceNode()->getName() << "();\n";
                s << "   if (Current > temp) Current = temp;\n";
            }
        }
        s << "return Current;\n}\n";
        foreach (Edge *edge, edgeList)
            if (edge->sourceNode()!=this)
                edge->WriteSource(h);

}

double AndNode::Simulate()
{
    double Current = AndValue;

    foreach (Edge *edge, edgeList)
    {
        if (edge->sourceNode()!=this)
        {
            double v = edge->sourceNode()->Simulate();
            if (Current>v) Current = v;
        }
    }
    setCurrent(Current);
    return Current;

}

QPainterPath AndNode::shape() const
{
QRectF r = boundingRect();
QPainterPath path,epath;
    epath.addEllipse(r);
    path.moveTo(r.left()+r.width()/2,r.top());
    path.lineTo(r.topLeft());
    path.lineTo(r.bottomLeft());
    path.lineTo(r.left()+r.width()/2,r.bottom());
    return path.united(epath);
}
