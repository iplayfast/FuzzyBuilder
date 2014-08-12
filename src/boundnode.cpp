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
    QRectF r = paintSetup(painter, option);
    painter->drawEllipse( r);
}

QRectF BoundNode::boundingRect() const
{
    int width = 6;
    int height = 6;
    qreal adjust = 2;
    return QRectF( -width - adjust, -height - adjust, 2 * width + adjust, 2 * height + adjust);

}

void BoundNode::WriteHeader(QTextStream &h)
{
    if (getHeaderBeenWritten()) return;
    setHeaderBeenWritten(true);
    foreach (Edge *edge, edgeList)
    {
        if (edge->getSource()!=this)   {
            edge->getSource()->WriteHeader(h);
            edge->WriteNodeInfo(h);
        }
    }
}

void BoundNode::WriteNodeInfo(QTextStream &s)
{

    Q_UNUSED(s);
}


void BoundNode::WriteSourcePlainGuts(QTextStream &s)
{
    //
    Q_UNUSED(s);
}

