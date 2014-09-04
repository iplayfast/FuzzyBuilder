

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <qevent.h>
#include <qnamespace.h>
#include <QMessageBox>
#include <QDebug>
#include "graphwidget.h"
#include "timernode.h"



TimerNode::TimerNode(GraphWidget *graphWidget) : Node(graphWidget)
{
    QRect exposedRect(graphWidget->mapToScene(0,0).toPoint(),graphWidget->viewport()->rect().size());
    //QRect exposedRect(ui->graphicsView->mapToScene(0,0).toPoint(), ui->graphicsView->viewport()->rect().size());
    setPos(20,20);
}

QRectF TimerNode::boundingRect() const
{
int width = 40;
int height = 20;
qreal adjust = 2;
return QRectF( -width - adjust, -height - adjust, 2 * width + adjust, 2 * height + adjust);
}

bool TimerNode::AllowAttach(Node *) const
{
    return true;
}

void TimerNode::WriteHeader(QTextStream &h)
{
    if (getHeaderBeenWritten()) return;
    setHeaderBeenWritten(true);
    h << "void " << getName() << "(void);\n";
    return;
}

void TimerNode::WriteNodeInfo(QTextStream &s)
{
    QString ps; ps.sprintf("!!%f!!%f\n",pos().rx(),pos().ry());
    s << "//!!fTimer!!" << getName() <<  ps;
}

void TimerNode::FunctionData(QString &Return, QString &Parameters, QString &FunctionReturn)
{
 Return = "void ";
 Parameters = "()";
 FunctionReturn = "return;";
}

void TimerNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    QRectF r = paintSetup(painter, option);
    QString v;
        v.sprintf("\n%05.5f",getCurrent());
    painter->drawPath(shape());
    painter->drawText(r,Qt::AlignHCenter,getName());
}

void TimerNode::setName(const QString &value)
{
    Node::setName(value);
}


QPainterPath TimerNode::shape() const
{
    QPainterPath path,epath;
    QRectF r= boundingRect();
    epath.addEllipse(r);
    //r.setWidth(r.width()/2);
    epath.moveTo(r.width()/2,r.height()/2);
    epath.lineTo(0,r.height()/3);
    epath.moveTo(r.width()/2,r.height()/2);
    epath.lineTo(r.width()/3,r.height()/2);
    //path.addRect(r);
    return epath.subtracted(path);


}
