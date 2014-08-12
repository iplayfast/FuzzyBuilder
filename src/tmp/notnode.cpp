#include "notnode.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <qevent.h>
#include <qnamespace.h>
#include <QDebug>

NotNode::NotNode(GraphWidget *graphWidget) : Node(graphWidget)
{
}

QPainterPath NotNode::shape() const
{
QRectF r = boundingRect();
QPainterPath path,epath;
int rad = r.width()/8,h2 = r.height()/2;
    path.moveTo(r.topLeft());
    path.lineTo(r.right()-(rad+rad),r.top()+h2);
    path.lineTo(r.left(),r.bottom());
    path.lineTo(r.topLeft());
    epath.addEllipse(QPointF(r.right()-rad,r.top()+h2),rad,rad);

    return path.united(epath);
}


QRectF NotNode::boundingRect() const
{
    int width = 20;
    int height = 20;
    qreal adjust = 2;
    return QRectF( -width - adjust, -height - adjust, 2 * width + adjust, 2 * height + adjust);

}

bool NotNode::AllowAttach(Node *) const
{
    return edgeList.count()<1;
}

void NotNode::WriteSource(QTextStream &h, QTextStream &s)
{
    if (getBeenWritten()) return;
    setBeenWritten(true);


    foreach (Edge *edge, edgeList)
    {
        if (edge->sourceNode()!=this)
            edge->sourceNode()->WriteSource(h,s);
    }
    QString ps; ps.sprintf("!!%f!!%f\n",pos().rx(),pos().ry());
    h << "//!!fNOT!!" << getName() << "!!" << ps;
    h << "double " << getName() << "(void);\n";
    s << "\ndouble " << getName() << "(void)\n{\n";

        foreach (Edge *edge, edgeList)
        {
            if (edge->sourceNode()!=this)
            {
                s << "   return 1.0 - " << edge->sourceNode()->getName() << "();\n";

            }
        }
        s << "\n}\n";
        foreach (Edge *edge, edgeList)
            if (edge->sourceNode()!=this)
                edge->WriteSource(h);
}

double NotNode::Simulate()
{
    foreach (Edge *edge, edgeList)
    {
        if (edge->sourceNode()!=this)
        {
            double v = edge->sourceNode()->Simulate();
            setCurrent(1.0 - v);
            return getCurrent();
        }
    }
    return getCurrent();
}

