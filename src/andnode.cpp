#include "andnode.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <qevent.h>
#include <qnamespace.h>
#include <QDebug>

#include "Util.h"
AndNode::AndNode(GraphWidget *graphWidget) : Node(graphWidget)
{
    AndValue = 1.0;
    QRect exposedRect(graphWidget->mapToScene(0,0).toPoint(),graphWidget->viewport()->rect().size());
    setPos(exposedRect.width()/2,exposedRect.height() / 2);
    if (!FindNewVertPosition(-1))
        FindNewVertPosition(1);

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


void AndNode::WriteHeader(QTextStream &h) const
{    
    h << "\ndouble " << getName() << "(void);\n";
}

void AndNode::WriteNodeInfo(QTextStream &s) const
{
         QString ps; ps.sprintf("!!%05.5f!!%f!!%f!!%f\n",getActiveValue(),pos().rx(),pos().ry(),getIOMin());
    s << "//!!fAnd!!" << getName()  << ps; // fAnd,name,active value,positionx,position y
    Node::WriteNodeInfo(s);
}

void AndNode::FunctionData(QString &Return, QString &Parameters, QString &FunctionReturn, bool &HasBrackets) const
{
    Return = "double ";
    Parameters = "()";
    FunctionReturn = " return Current;";
    HasBrackets = true;
}



QString AndNode::Regenerate() const
{
    QString s,ss;
        ss.sprintf("%05.5f",getActiveValue());

    s = "{\n";
    s += "double  temp,Current = "; s+= ss; s+= ";\n";
        foreach (Edge *edge, edgeList)
        {
            if (edge->getSource()!=this)
            {
                s += "   temp = "; s+= edge->getSource()->getName(); s+= "();\n";
                s += "   if (Current > temp) Current = temp;\n";
            }
        }
        return s;
}

QString AndNode::MinText() const
{
    return "Maximum possible Output";
}


double AndNode::Simulate()
{    
    double Current = AndValue;

    foreach (Edge *edge, edgeList)
    {
        if (edge->getSource()!=this)
        {
            double v = edge->getSource()->Simulate();
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
