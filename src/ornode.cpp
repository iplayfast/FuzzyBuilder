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
ActiveValue = 0;

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

void OrNode::WriteHeader(QTextStream &h)
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

void OrNode::WriteNodeInfo(QTextStream &s)
{
        StartComment(s);
        s << "* Or Node will find the maximum of a set of inputs\n";

    QString ps; ps.sprintf("!!%f!!%f\n",pos().rx(),pos().ry());
    QString ss;
        ss.sprintf("%05.5f",ActiveValue);
    s << "//!!fOr!!" << getName() << "!!" << ss << ps;
    foreach (Edge *edge, edgeList)
        if (edge->getSource()!=this)
            edge->WriteNodeInfo(s);
    EndComment(s);
}

void OrNode::FunctionData(QString &Return, QString &Parameters, QString &FunctionReturn)
{
    Return = "double ";
    Parameters = "()";
    FunctionReturn = " return Current;";
}

void OrNode::WriteSourcePlainGuts(QTextStream &ts)
{
    if (getUserGuts()=="")
        ts << Regenerate();
    else ts << getUserGuts();
}

QString OrNode::Regenerate()
{

    QString ss;
        ss.sprintf("%05.5f;\n",ActiveValue);
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
