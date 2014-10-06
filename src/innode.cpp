
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <qevent.h>
#include <qnamespace.h>
#include <QDebug>
#include "graphwidget.h"
#include "innode.h"

InNode::InNode(GraphWidget *graphWidget) : Node(graphWidget)
{
    QRect exposedRect(graphWidget->mapToScene(0,0).toPoint(),graphWidget->viewport()->rect().size());
    //QRect exposedRect(ui->graphicsView->mapToScene(0,0).toPoint(), ui->graphicsView->viewport()->rect().size());
    setPos(0,exposedRect.height() / 2);
    if (!FindNewVertPosition(-1))
        FindNewVertPosition(1);

    setIOMax(255);
    setIOMin(0);
    this->setCurrent(0);
    this->setActive(0);
    Regenerate();
}
QString InNode::Regenerate() const
{
    QString b;
    QString s;
    QString min,max;
    min.sprintf("%05.5f",getIOMin());
    max.sprintf("%05.5f",getIOMax());

    s = "double RealWorldMin = "; s+= min; s+= ",RealWorldMax = "; s+= max; s+= ";\n";


    b = "double myinput = 0; /*add your code here to obtain input from a sensor or something*/\n";
    b += s;
    b += "if (myinput<RealWorldMin) myinput = RealWorldMin; // clamp lower limit\n";
    b += "if (myinput>RealWorldMax) myinput = RealWorldMax; // clamp upper limit\n";
    return b;
};
QRectF InNode::boundingRect() const
{
int width = getWidth();
int height = getHeight();
qreal adjust = 2;
return QRectF( -width - adjust, -height - adjust, 2 * width + adjust, 2 * height + adjust);
}

bool InNode::AllowAttach(Node *) const
{
    return false;
}

void InNode::WriteHeader(QTextStream &h) const
{
    h << "double " << getName() << "(void);\n";
}

void InNode::WriteNodeInfo(QTextStream &ts) const
{
    QString ps; ps.sprintf("!!%f!!%f\n",pos().rx(),pos().ry());
    ts << "\n";
    ts << "//!!fIn!!" << getName() <<  ps;
    Node::WriteNodeInfo(ts);
}

void InNode::FunctionData(QString &Return, QString &Parameters, QString &FunctionReturn) const
{
    Return = "double ";
    Parameters = "()";
    FunctionReturn = "return myinput;";
}





void InNode::WriteSourcePlainGuts(QTextStream &ts) const
{

    if (getUserGuts()=="")
        ts << Regenerate();
    else ts << getUserGuts();
}

double InNode::Simulate()
{
    double v = Normalize(getInValue());
    setCurrent(v);
    return v;
}

QPainterPath InNode::shape() const
{
    QPainterPath path,epath;
    QRectF r= boundingRect();

    epath.addEllipse(r);
    r.setWidth(r.width()/2);

    path.addRect(r);
    return epath.subtracted(path);
}

int InNode::MaxOfMin() const
{
    return 256 * this->getMinScale();
}

int InNode::MaxOfMax() const
{
    return 256 * this->getMaxScale();
}

QString InNode::ExtraText() const
{
QString s;
    s.sprintf("Simulated Input %f -> %f <- %f",getIOMin(),getActiveValue(),getIOMax());
    return s;
}

void InNode::setIOMin(double value)
{
    if (getIOMax()<value)
        setIOMax(value);
    Node::setIOMin(value);
}

void InNode::setIOMax(double value)
{
    if (value<1) value = 1;
    if (getIOMin()>=value)
           setIOMin(value-1);
    Node::setIOMax(value);

}

QString InNode::GetValueText() const
{
QString v = Node::GetValueText();
QString v1;
    v1.sprintf("\n(%d)",(int)round(this->getActiveValue()));
    v1 += v;
    return v1;
}

/*QString InNode::MaxText()
{
    return FormatLabel("Max",1.0 * MaxMin(),1.0*ActiveValue,1.0 * MaxMax());
}
*/
