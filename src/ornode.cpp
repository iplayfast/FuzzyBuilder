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
    setActiveValue(0);
QRect exposedRect(graphWidget->mapToScene(0,0).toPoint(),graphWidget->viewport()->rect().size());
setPos(exposedRect.width()/2,exposedRect.height() / 2);
if (!FindNewVertPosition(-1))
    FindNewVertPosition(1);

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

void OrNode::WriteHeader(QTextStream &h) const
{
    h << "double " << getName() << "(void);\n";
}

void OrNode::WriteNodeInfo(QTextStream &s) const
{
    QString ps; ps.sprintf("!!%f!!%f\n",pos().rx(),pos().ry());
    QString ss;
        ss.sprintf("%05.5f",getActiveValue());
    s << "//!!fOr!!" << getName() << "!!" << ss << ps;
    Node::WriteNodeInfo(s);
}

QString OrNode::MinText() const
{
    return  "The lowest possible value";
}

void OrNode::FunctionData(QString &Return, QString &Parameters, QString &FunctionReturn) const
{
    Return = "double ";
    Parameters = "()";
    FunctionReturn = " return Current;";
}

void OrNode::WriteSourcePlainGuts(QTextStream &ts) const
{
    if (getUserGuts()=="")
        ts << Regenerate();
    else ts << getUserGuts();
}

QString OrNode::Regenerate() const
{

    QString ss;
        ss.sprintf("%05.5f;\n",getActiveValue());
    QString s;
    s = "double  temp,Current = "; s +=  ss;
        foreach (Edge *edge, edgeList)
        {
            if (edge->getSource()!=this)
            {
                s += "   temp = "; s+= edge->getSource()->getName(); s+= "();\n";
                s += "   if (Current < temp) Current = temp;\n";
            }
        }
    return s;
}

double OrNode::Simulate()
{
double    Current = 0.0;
    foreach (Edge *edge, edgeList)
    {
        if (edge->getSource()!=this)
        {
            double v = edge->getSource()->Simulate();
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
