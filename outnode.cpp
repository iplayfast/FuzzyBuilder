#include "outnode.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <qevent.h>
#include <qnamespace.h>
#include <QDebug>
#include "graphwidget.h"

void OutNode::drawShape(QPainter *p, QRectF &r)
{
    QRectF a(r);
    a.adjust(r.width()/2,0,0,0);
   p->drawEllipse(a);
}
QColor OutNode::getGradient(int zero_one)
{
    if (zero_one)
        return QColor(Qt::red);
    return QColor(Qt::darkRed);
}

OutNode::OutNode(GraphWidget *graphWidget) : Node(graphWidget)
{
    //QRect exposedRect(ui->graphicsView->mapToScene(0,0).toPoint(), ui->graphicsView->viewport()->rect().size());
    QRect exposedRect(graphWidget->mapToScene(0,0).toPoint(),graphWidget->viewport()->rect().size());
    setPos(exposedRect.width(),exposedRect.height() / 2);

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

void OutNode::WriteSource(QTextStream &h,QTextStream &s)
{
    if (getBeenWritten()) return;
    setBeenWritten(true);


    foreach (Edge *edge, edgeList)
    {
        if (edge->sourceNode()!=this)
        {
            edge->sourceNode()->WriteSource(h,s); // only one edge on output
            QString ps; ps.sprintf("!!%f!!%f\n",pos().rx(),pos().ry());
            h << "//!!fOut!!" << getName() << ps;

            h << "/* This is your entry point into the code, that is, call this routine to get the conditioned value from any inputs */\n";
            h << "void " << getName() << "(void);\n";
            s << "\n\n\n/* This is your entry point into the code, that is, call this routine to get the conditioned value from any inputs */\n";
            s << "void " << getName() << "(void)\n";
            s << "{\n";
            s << "int Max = " << IOMax << ",Min = " << IOMin << ";\n\n";
            s << "/*convert from range [0..1] to real world values */";
            s << "  result = ((Max-Min) * " << edge->sourceNode()->getName() << "())+Min;\n";
            s << "/* add code here to send result to where you want it*/\n#error add your code\n";
            s << "\n}\n\n";

            foreach (Edge *edge, edgeList)
                if (edge->sourceNode()!=this)
                    edge->WriteSource(h);

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
        if (edge->sourceNode()!=this)
        {
            double v = edge->sourceNode()->Simulate();
            Current = (IOMax - IOMin) * v + IOMin;
            setCurrent(Current);
            return Current;
        }
    }
    setCurrent(Current);
    return Current;
}
