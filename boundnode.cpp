#include "boundnode.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <qevent.h>
#include <qnamespace.h>
#include <QDebug>

BoundNode::BoundNode(GraphWidget *graphWidget) : Node(graphWidget)
{
}

bool BoundNode::AllowAttach(Node *) const
{
    return edgeList.count()<1;
}

void BoundNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget */*widget*/)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::darkGray);
QRectF r =  boundingRect();
    //r.adjust(-3,-3,0,0);
    painter->drawEllipse(r);
    //r.adjust(3,3,0,0);
    QRadialGradient gradient(r.center(),r.height());
    if (option->state & QStyle::State_Sunken) {
        gradient.setCenter(3, 3);
        gradient.setFocalPoint(3, 3);
        if (!getSelected()) {
            gradient.setColorAt(1, QColor(Qt::yellow).light(120));
            gradient.setColorAt(0, QColor(Qt::darkYellow).light(120));
        }
        else {
            gradient.setColorAt(1, QColor(Qt::green).light(120));
            gradient.setColorAt(0, QColor(Qt::darkGreen).light(120));
        }
    } else {
        if (!getSelected()) {
            gradient.setColorAt(0, Qt::yellow);
            gradient.setColorAt(1, Qt::darkYellow);
        }
        else {
            gradient.setColorAt(1, QColor(Qt::green));
            gradient.setColorAt(0, QColor(Qt::darkGreen));
        }
    }
    painter->setBrush(gradient);
}

QRectF BoundNode::boundingRect() const
{
    int width = 6;
    int height = 6;
    qreal adjust = 2;
    return QRectF( -width - adjust, -height - adjust, 2 * width + adjust, 2 * height + adjust);

}

void BoundNode::WriteSource(QTextStream &h, QTextStream &s)
{
    if (getBeenWritten()) return;
    setBeenWritten(true);
    foreach (Edge *edge, edgeList)
    {
        if (edge->sourceNode()!=this)   {
            edge->sourceNode()->WriteSource(h,s);
            edge->WriteSource(h);
        }
    }
}

