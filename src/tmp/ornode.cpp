#include "ornode.h"
#include "graphwidget.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <qevent.h>
#include <qnamespace.h>
#include <QDebug>
#include <QIcon>



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

QPainterPath OrNode::shape() const
{
    QPainterPath path;
    QRectF r =  boundingRect();
    float x = r.left();
    float y = r.top();
    double xn = r.width() / 5.6;
    double yn = r.height() / 4;

    path.moveTo(x,y);
    path.lineTo(x+xn*3,y+yn*0);
        path.lineTo(x+xn*4,y+yn*0.4);
            path.lineTo(x+xn*5,y+yn*1);
                path.lineTo(x+xn*5.6,y+yn*2);
            path.lineTo(x+xn*5,y+yn*3);
        path.lineTo(x+xn*4,y+yn*3.6);
    path.lineTo(x+xn*3,y+yn*4);
    path.lineTo(x+xn*0,y+yn*4);

    path.lineTo(x+xn*1,y+yn*3.4);
    path.lineTo(x+xn*1.5,y+yn*2);
    path.lineTo(x+xn*1,y+yn*1);
    path.lineTo(x,y);
    return path;
}
