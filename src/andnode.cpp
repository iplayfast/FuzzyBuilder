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


void AndNode::WriteHeader(QTextStream &h)
{
    if (getHeaderBeenWritten()) return;
    setHeaderBeenWritten(true);
    h << "\ndouble " << getName() << "(void);\n";
    foreach (Edge *edge, edgeList)
    {
        if (edge->getSource()!=this)
            edge->getSource()->WriteHeader(h);
    }
}

void AndNode::WriteNodeInfo(QTextStream &s)
{
    StartComment(s);
    s << "* And Node will find the minimum of a set of inputs\n";
    s << "* Fuzzy builder Description\n";
         QString ps; ps.sprintf("!!%05.5f!!%f!!%f\n",ActiveValue,pos().rx(),pos().ry());
    s << "//!!fAnd!!" << getName()  << ps; // fAnd,name,active value,positionx,position y
    foreach (Edge *edge, edgeList)
        if (edge->getSource()!=this)
            edge->WriteNodeInfo(s);
    Node::WriteNodeInfo(s);
    EndComment(s);
}



void AndNode::WriteSourcePlainGuts(QTextStream &s)
{
        if (getUserGuts()=="")
            s << Regenerate();
        else s << getUserGuts();
}

void AndNode::FunctionData(QString &Return, QString &Parameters, QString &FunctionReturn)
{
    Return = "double ";
    Parameters = "()";
    FunctionReturn = " return Current;";
}

QString AndNode::Regenerate()
{
    QString s,ss;
        ss.sprintf("%05.5f",ActiveValue);

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

QString AndNode::MinText()
{
    return FormatLabel("Maximum Output",1.0 * MaxMin() / NODEHIGHVAL,IOMin,1.0*MaxMax() / NODEHIGHVAL);
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
