
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

    IOMax = 1;
    IOMin = 0;
    Regenerate();
}
QString InNode::Regenerate()
{
    QString b;
    QString s;
    QString min,max;
    min.sprintf("%05.5f",IOMin);
    max.sprintf("%05.5f",IOMax);

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

void InNode::WriteHeader(QTextStream &h)
{
    if (getHeaderBeenWritten()) return;
    setHeaderBeenWritten(true);

    h << "double " << getName() << "(void);\n";
    return;

}

void InNode::WriteNodeInfo(QTextStream &ts)
{

    QString ps; ps.sprintf("!!%f!!%f\n",pos().rx(),pos().ry());
    ts << "\n";
    ts << "//!!fIn!!" << getName() <<  ps;
    Node::WriteNodeInfo(ts);
}

void InNode::FunctionData(QString &Return, QString &Parameters, QString &FunctionReturn)
{
    Return = "double ";
    Parameters = "()";
    FunctionReturn = "return myinput;";
}





void InNode::WriteSourcePlainGuts(QTextStream &ts)
{

    if (getUserGuts()=="")
        ts << Regenerate();
    else ts << getUserGuts();
}

double InNode::Simulate()
{
    double v = (InValue - IOMin) / (IOMax - IOMin); // normalize
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
