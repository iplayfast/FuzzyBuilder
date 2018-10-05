/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "edge.h"
#include "node.h"

#include <math.h>

#include <QPainter>

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

//! [0]
Edge::Edge(Node *sourceNode, Node *destNode)
    : arrowSize(10)
{
    //setAcceptedMouseButtons(0);
    source = sourceNode;
    dest = destNode;
    source->addEdge(this);
    dest->addEdge(this);
    adjust();
}
//! [0]

//! [1]
Node *Edge::sourceNode() const
{
    return source;
}

Node *Edge::destNode() const
{
    return dest;
}
//! [1]

//! [2]
void Edge::adjust()
{
    if (!source || !dest)
        return;

    QLineF line(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
    qreal length = line.length();

    prepareGeometryChange();
QRectF rs(source->boundingRect()),rd(dest->boundingRect());
    if (length > qreal(20.)) {

        QPointF edgeOffset((line.dx() * 10) / length, (line.dy() * 10) / length);
        sourcePoint = line.p1();
        destPoint = line.p2();
   //     if (line.p1().x()<line.p2().x())
        {
            sourcePoint.setX(sourcePoint.x() + rs.right());
            destPoint.setX(destPoint.x() + rd.left());
        }
/*        else {
                sourcePoint.setX(sourcePoint.x() + rs.left());
                destPoint.setX(destPoint.x() + rd.right());
        }
*/
        //sourcePoint = line.p1() + rs.topLeft();//edgeOffset;
        //destPoint = line.p2() - rd.topLeft();//edgeOffset;
    } else {
        sourcePoint = destPoint = line.p1();
    }
}

void Edge::WriteSource(QTextStream &h)
{
    h << "//!!fEdge!!" << this->sourceNode()->getName() << "!!" << this->destNode()->getName() << "!!\n";
}
//! [2]

//! [3]
QRectF Edge::boundingRect() const
{
    if (!source || !dest)
        return QRectF();

    qreal penWidth = 1;
    qreal extra = 5+(penWidth + arrowSize) / 2.0;

    return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                      destPoint.y() - sourcePoint.y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);

}
//! [3]

//! [4]
void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!source || !dest)
        return;

    QLineF line(sourcePoint, destPoint);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;
//! [4]

//! [5]
    // Draw the line itself
    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);

//! [5]

//! [6]
    // Draw the arrows
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
        angle = TwoPi - angle;

    //QPointF sourceArrowP1 = sourcePoint + QPointF(sin(angle + Pi / 3) * arrowSize, cos(angle + Pi / 3) * arrowSize);
    //QPointF sourceArrowP2 = sourcePoint + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,cos(angle + Pi - Pi / 3) * arrowSize);
    QPointF destArrowP1 = destPoint + QPointF(sin(angle - Pi / 3) * arrowSize,cos(angle - Pi / 3) * arrowSize);
    QPointF destArrowP2 = destPoint + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,cos(angle - Pi + Pi / 3) * arrowSize);

    painter->setBrush(Qt::black);
    //painter->drawPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);
    painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
    //painter->drawRect(boundingRect());

    QString s = dest->getINTEXT();
    if (s!="") {
        qreal angleInDegrees = atan2(line.dy(),line.dx()) * 180 / 3.14;
        QRectF br(painter->fontMetrics().boundingRect(s));


        //function getPointAtDistance( p1:Point, p2:Point, dis:Number ) : Point

           //get vector
           double x3 = line.p2().x() - line.p1().x();
           double y3 = line.p2().y() - line.p1().y();

           //normalize vector
           double length = sqrt( x3 * x3 + y3 * y3 );
           x3 /= length;
           y3 /= length;

           //scale vector
           x3 *= line.length()-br.width()-10;
           y3 *= line.length()-br.width()-10;

           //add vector back onto initial point and return
           QPointF p( line.p1().x() + x3, line.p1().y() + y3 );


        this->drawRotatedText(painter,angleInDegrees,p.x(),p.y(),s);

        //this->drawRotatedText(painter,angleInDegrees,line.p2().x()-br.width(),line.p2().y()+br.height(),s);
    }
}
void Edge::drawRotatedText(QPainter *painter, float degrees, int x, int y, const QString &text)
  {
    painter->save();
    painter->setClipping(false);
    painter->translate(x, y);
    painter->rotate(degrees);
    painter->drawText(0, 0, text);
    painter->restore();
  }
void Edge::mousePressEvent(QGraphicsSceneMouseEvent */*event*/)
{
    /*int src = sourceNode()->edges().indexOf(this);
    int dest= this->destNode()->edges().indexOf(this);
    sourceNode()->edges().removeOne(this);
    destNode()->edges().removeOne(this);
    source = 0;
    dest = 0;*/
//    delete this;
}
//! [6]
