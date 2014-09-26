#include "outnode.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <qevent.h>
#include <qnamespace.h>
#include <QDebug>
#include "graphwidget.h"
OutNode::OutNode(GraphWidget *graphWidget) : Node(graphWidget)
{
    //QRect exposedRect(ui->graphicsView->mapToScene(0,0).toPoint(), ui->graphicsView->viewport()->rect().size());
    QRect exposedRect(graphWidget->mapToScene(0,0).toPoint(),graphWidget->viewport()->rect().size());
    Node::setIOMax(1.0);
    Node::setIOMin(0.0);
    setPos(exposedRect.width(),exposedRect.height() / 2);
    if (!FindNewVertPosition(-1))
        FindNewVertPosition(1);

    Regenerate();
}

QString OutNode::Regenerate()
{
    QString s;
    setUserGuts(s);    // clear out old data
    foreach (Edge *edge, edgeList)
    {
        if (edge->getSource()!=this) // only one possible
        {
            QString min,max;
            min.sprintf("%05.5f",getIOMin());
            max.sprintf("%05.5f",getIOMax());
            s = "/* Call this routine to get the conditioned value from any inputs */\n";
            s += "double RealWorldMin = "; s+= min; s+= ",RealWorldMax = "; s+= max; s+= ";\n";
            s += "/*convert from range [0..1] to real world values */\n";
            s += "int result = ((RealWorldMax-RealWorldMin) * "; s+= edge->getSource()->getName(); s+= "())+RealWorldMin;\n";
            break;
        }
    }
    s +=  "    if (result>RealWorldMax) result = RealWorldMax;// clamp upper limit\n";
    s += "    if (result<RealWorldMin) result = RealWorldMin;// clamp lower limit\n";
    return s;
}

QRectF OutNode::boundingRect() const
{
int width = 40;
int height = 20;
qreal adjust = 2;
return QRectF( -width - adjust, -height - adjust, 2 * width + adjust, 2 * height + adjust);
}

bool OutNode::AllowAttach(Node *) const
{
    return (edgeList.count()<1);
}


void OutNode::WriteHeader(QTextStream &h)
{
    if (getHeaderBeenWritten()) return;
    setHeaderBeenWritten(true);

    h << "void " << getName() << "(void);\n";
}

void OutNode::FunctionData(QString &Return, QString &Parameters, QString &FunctionReturn)
{
    Return = "void ";
    Parameters = "()";
    FunctionReturn = "return; ";
}




void OutNode::WriteSourcePlainGuts(QTextStream &ts)
{
    if (getSourceBeenWritten()) return;
    setSourceBeenWritten(true);

    QString s;
    s = Regenerate();
    if (getUserGuts()=="")
        ts << s;
    else ts << getUserGuts();
    return;
}

void OutNode::WriteNodeInfo(QTextStream &s)
{
    foreach (Edge *edge, edgeList)
    {
        if (edge->getSource()!=this)
        {
            edge->getSource()->WriteNodeInfo(s); // only one edge on output
            StartComment(s);
            QString ps; ps.sprintf("!!%f!!%f\n",pos().rx(),pos().ry());
            s << "\n\n";
            s << "//!!fOut!!" << getName() << ps;
            Node::WriteNodeInfo(s);
            EndComment(s);
            Node::WriteSourceUserGuts(s);
            return;
        }
    }
    return;
}


double OutNode::Simulate()
{
double    Current = 0.0;
    foreach (Edge *edge, edgeList)
    {
        if (edge->getSource()!=this)
        {
            double v = edge->getSource()->Simulate();
            Current = (getIOMax() - getIOMin()) * v + getIOMin();
            setCurrent(Current);
            return Current;
        }
    }
    setCurrent(Current);
    this->update();
    return Current;
}

QPainterPath OutNode::shape() const
{
    QPainterPath path,epath;
    QRectF r= boundingRect();
    epath.addEllipse(r);
    r.adjust(r.width()/2,0,0,0);
    path.addRect(r);
    return epath.subtracted(path);
}

int OutNode::MaxOfMin() const
{
    return 256 * this->getMinScale();
}

int OutNode::MaxOfMax() const
{
    return 256 * this->getMaxScale();
}
