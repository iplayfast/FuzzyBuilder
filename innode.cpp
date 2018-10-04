
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <qevent.h>
#include <qnamespace.h>
#include <QDebug>
#include "graphwidget.h"
#include "innode.h"

void InNode::drawShape(QPainter *p, QRectF &r)
{
    QRectF a(r);
    a.adjust(-r.width()/2,0,0,0);
   p->drawEllipse(a);
}

InNode::InNode(GraphWidget *graphWidget) : Node(graphWidget)
{
    QRect exposedRect(graphWidget->mapToScene(0,0).toPoint(),graphWidget->viewport()->rect().size());
    //QRect exposedRect(ui->graphicsView->mapToScene(0,0).toPoint(), ui->graphicsView->viewport()->rect().size());
    setPos(0,exposedRect.height() / 2);
    //setPos(exposedRect.left(),exposedRect.height() );
}

QRectF InNode::boundingRect() const
{
int width = 40;
int height = 20;
qreal adjust = 2;
return QRectF( -width - adjust, -height - adjust, 2 * width + adjust, 2 * height + adjust);
}

bool InNode::AllowAttach(Node *) const
{
    return false;
}



void InNode::WriteSource(QTextStream &h,QTextStream &s)
{
    if (getBeenWritten()) return;


    setBeenWritten(true);
    QString ps; ps.sprintf("!!%f!!%f\n",pos().rx(),pos().ry());
    h << "//!!fIn!!" << getName() <<  ps;
    h << "double " << getName() << "(void);\n";
    s << "double " << getName() << "(void)\n{\n";
    s << "int Max = " << IOMax << ",Min = " << IOMin << ";\n";
    s << "double myinput = 0; /*add your code here to obtain input from a sensor or something*/\n";
    s << "   myinput = (myinput - Min) / (Max - Min);/* convert input to a 0 <=Number <=1 */\n";
    s << "   return myinput;\n}\n\n";
    return;
}

double InNode::Simulate()
{
    double v = (InValue - IOMin) / (IOMax - IOMin); // normalize
    setCurrent(v);
    return v;
}
