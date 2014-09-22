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
    QRect exposedRect(graphWidget->mapToScene(0,0).toPoint(),graphWidget->viewport()->rect().size());
    setPos(exposedRect.width()/2,exposedRect.height() / 2);
    if (!FindNewVertPosition(-1))
        FindNewVertPosition(1);

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

void NotNode::WriteHeader(QTextStream &h)
{
    if (getHeaderBeenWritten()) return;
    setHeaderBeenWritten(true);
    h << "double " << getName() << "(void);\n";
    foreach (Edge *edge, edgeList)
    {
        if (edge->getSource()!=this)
            edge->getSource()->WriteHeader(h);
    }
}

void NotNode::WriteNodeInfo(QTextStream &s)
{
    StartComment(s);
    s << "* Not will return the opposite fuzzy value of an input (0 becomes 1, 1 becomes 0 etc)\n";
    QString ps; ps.sprintf("!!%f!!%f\n",pos().rx(),pos().ry());
    s << "//!!fNOT!!" << getName() << "!!" << ps;
    Node::WriteNodeInfo(s);
    EndComment(s);
}

void NotNode::FunctionData(QString &Return, QString &Parameters, QString &FunctionReturn)
{
    Return = "double ";
    Parameters = "()";
    FunctionReturn = "return 1.0 - Value;";
}


void NotNode::WriteSourcePlainGuts(QTextStream &s)
{
         foreach (Edge *edge, edgeList)
         {
             if (edge->getSource()!=this)
             {
                 s << "   double value = " << edge->getSource()->getName() << "();\n";
                return;
             }
         }
         s << "\n";
}

double NotNode::Simulate()
{
    foreach (Edge *edge, edgeList)
    {
        if (edge->getSource()!=this)
        {
            double v = edge->getSource()->Simulate();
            setCurrent(1.0 - v);
            return getCurrent();
        }
    }
    return getCurrent();
}

