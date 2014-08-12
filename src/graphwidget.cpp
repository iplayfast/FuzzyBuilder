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

#include "graphwidget.h"
#include "edge.h"
#include "nodefactory.h"

#include <math.h>
#include <QGraphicsSceneEvent>
#include <QKeyEvent>
class MyQGraphicsScene : public QGraphicsScene
{
    GraphWidget *widget;
public:
    MyQGraphicsScene(GraphWidget *w,QWidget *parent) :QGraphicsScene(parent) { widget = w; }
    void mousePressEvent(QGraphicsSceneMouseEvent *event)
    {
            QGraphicsScene::mousePressEvent(event);
            if (!event->isAccepted())
                widget->mousePressEvent(event);
    }

};

//! [0]
GraphWidget::GraphWidget(QWidget *parent)
    : QGraphicsView(parent), timerId(0)
{
    MyQGraphicsScene *scene = new MyQGraphicsScene(this,parent);//this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);

    Shift = false;
    //scene->setSceneRect(-parent->width(),- parent->height(),parent->width(),parent->height());
    scene->setSceneRect(0,0,parent->width(),parent->height());
    setScene(scene);
    setCacheMode(CacheNone);//CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(0.95), qreal(0.95));
    setMinimumSize(400, 400);
    setWindowTitle(tr("Fuzzy Builder"));
#ifdef shit
//! [0]
//! [1]
    Node *node1 = NodeFactory::Create(this,fIN);
    node1->IOMin = 0;
    node1->IOMax = 5;
    Node *node2 = NodeFactory::Create(this,fOUT);
    node2->IOMin = 3;
    node2->IOMax = 5;
    node1->setName("sensor");
    node2->setName("buzzer");
    scene->addItem(node1);
    scene->addItem(node2);
    scene->addItem(new Edge(node1, node2));



    Node *node3 = new Node(this,fFUZZY);
    node3->Name = "fuzball";
    Node *node4 = new Node(this,fOR);
    node4->Name = "bob";
    //centerNode = new Node(this,fAND);
    //centerNode->Name = "george";
    Node *node6 = new Node(this,fOUT);
    node6->Name = "out1";


    /*Node *node7 = new Node(this);
    Node *node8 = new Node(this);
    Node *node9 = new Node(this);
*/
    scene->addItem(node3);
    scene->addItem(node4);
    scene->addItem(node6);
    /*scene->addItem(centerNode);*/
  /*  scene->addItem(node6);
    scene->addItem(node7);
    scene->addItem(node8);
    scene->addItem(node9);
*/

    //scene->addItem(new Edge(node1, node2));
/*    scene->addItem(new Edge(node1, node3));
    scene->addItem(new Edge(node1, node8));
    scene->addItem(new Edge(node2, node3));
    scene->addItem(new Edge(node2, centerNode));
    scene->addItem(new Edge(node3, node6));
    scene->addItem(new Edge(node4, node1));
    scene->addItem(new Edge(node4, centerNode));
    scene->addItem(new Edge(centerNode, node6));
    scene->addItem(new Edge(centerNode, node8));
    scene->addItem(new Edge(node6, node9));
    scene->addItem(new Edge(node7, node4));
    scene->addItem(new Edge(node8, node7));
    scene->addItem(new Edge(node9, node8));
*/
    /*node1->setPos(-50, -50);
    node2->setPos(0, -50);
    node3->setPos(50, -50);
    node4->setPos(-50, 0);*/
    //centerNode->setPos(0, 0);
/*    node6->setPos(50, 0);
    node7->setPos(-50, 50);
    node8->setPos(0, 50);
    node9->setPos(50, 50);
*/
#endif
}
//! [1]

//! [2]
void GraphWidget::itemMoved()
{
    if (!timerId)
        timerId = startTimer(1000 / 25);
}
//! [2]

//! [3]
void GraphWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    /*case Qt::Key_Up:
        centerNode->moveBy(0, -20);
        break;
    case Qt::Key_Down:
        centerNode->moveBy(0, 20);
        break;
    case Qt::Key_Left:
        centerNode->moveBy(-20, 0);
        break;
    case Qt::Key_Right:
        centerNode->moveBy(20, 0);
        break;*/
    case Qt::Key_Plus:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    case Qt::Key_Space:
    case Qt::Key_Enter:
        shuffle();
        break;
   case Qt::Key_Shift:
        Shift = true;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}

void GraphWidget::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() ==Qt::Key_Shift)
         Shift = false;
    QGraphicsView::keyPressEvent(event);
}
//! [3]

//! [4]
void GraphWidget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    QList<Node *> nodes;
    foreach (QGraphicsItem *item, scene()->items()) {
        if (Node *node = qgraphicsitem_cast<Node *>(item))
            nodes << node;
    }

    foreach (Node *node, nodes)
        node->calculateForces();

    bool itemsMoved = false;
    foreach (Node *node, nodes) {
        if (node->advance())
            itemsMoved = true;
    }

    if (!itemsMoved) {
        killTimer(timerId);
        timerId = 0;
    }
}

void GraphWidget::mousePressEvent(QGraphicsSceneMouseEvent */*event*/)
{
    QList<Node *> nodes;
    foreach (QGraphicsItem *item, scene()->items()) {
        if (Node *node = qgraphicsitem_cast<Node *>(item)){
            node->setSelected(false);
            node->update();
        }
    }

}

QString GraphWidget::SuggestName(LOGICTYPE t) const
{
    QList<Node *> nodes;
    foreach (QGraphicsItem *item, scene()->items()) {
        if (Node *node = qgraphicsitem_cast<Node *>(item))
            nodes << node;
    }

    QString name;
        switch(t)
        {
        case fSETUP:
            name = "setup";
            return name;
            break;
        case fIN:
            name = "In";
            break;
        case fOUT:
            name = "Out";
            break;
        case fAND:
            name = "And";
            break;
        case fOR:
            name = "Or";
            break;
        case fNOT:
            name = "Not";
            break;
        case fFUZZY:
            name = "Fuzzy";
            break;
        case fPID:
            name = "Pid";
            break;
        case fTIMER:
            name = "Timer";
            break;

        }
        int count = 1;
        QString result;
        result = name;
        result += QString::number(count);
        bool restart = true;
        while(restart) {
            restart = false;
            foreach (Node *node, nodes)
            {
                if (node->getName()==result)
                {
                    count++;
                    result = name;
                    result += QString::number(count);
                    restart = true;
                    break;
                }
            }
        }
        return result;

}
//! [4]

#ifndef QT_NO_WHEELEVENT
//! [5]
void GraphWidget::wheelEvent(QWheelEvent *event)
{
    scaleView(pow((double)2, -event->delta() / 240.0));
}
//! [5]
#endif

//! [6]
void GraphWidget::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);

    // Shadow
    QRectF sceneRect = this->sceneRect();
    QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
    QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);
    if (rightShadow.intersects(rect) || rightShadow.contains(rect))
    painter->fillRect(rightShadow, Qt::darkGray);
    if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
    painter->fillRect(bottomShadow, Qt::darkGray);

    // Fill
    QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, Qt::lightGray);
    painter->fillRect(rect.intersected(sceneRect), gradient);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(sceneRect);

    // Text
    QRectF textRect(sceneRect.left() + 4, sceneRect.top() + 4,
                    sceneRect.width() - 4, sceneRect.height() - 4);
/*    QString message(tr("Click and drag the nodes around, and zoom with the mouse "
                       "wheel or the '+' and '-' keys"));

    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(14);
    painter->setFont(font);
    painter->setPen(Qt::lightGray);
    painter->drawText(textRect.translated(2, 2), message);
    painter->setPen(Qt::black);
    painter->drawText(textRect, message);
    */
}
//! [6]

//! [7]
void GraphWidget::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}

void GraphWidget::simulate()
{
    QList<Node *> nodes;
    foreach (QGraphicsItem *item, scene()->items()) {
        if (Node *node = qgraphicsitem_cast<Node *>(item))
            nodes << node;
    }

    foreach (Node *node, nodes) {
        if (node->GetLogicType()==fOUT)
                node->Simulate();
    }
    foreach (Node *node, nodes) {
            node->update();
    }

}
void GraphWidget::ReadSource(QTextStream &h)
{
    NodeFactory::ReadSource(this,h);
}

Node *GraphWidget::FindNode(QString &name)
{
    foreach (QGraphicsItem *item, scene()->items()) {
        if (Node *node = qgraphicsitem_cast<Node *>(item))  {
            if (node->getName()==name)
                return node;
        }
    }
    return 0;
}

void GraphWidget::WriteSource(QTextStream &tsh, QTextStream &tss)
{
    QList<Node *> nodes;
    foreach (QGraphicsItem *item, scene()->items()) {
        if (Node *node = qgraphicsitem_cast<Node *>(item))
            nodes << node;
    }
    foreach (Node *node, nodes) {
        node->setSourceBeenWritten(false);
        node->setHeaderBeenWritten(false);
    }

    StartComment(tss);
    tss << "This is the data used for FuzzyBuilder to regenerate the layout\n";
    EndComment(tss);
    foreach(Node *node,nodes) {
        node->WriteNodeInfo(tss);
    }
    StartComment(tss);
    tss << "End of section\n";
    EndComment(tss);

    foreach (Node *node,nodes) {
        if (node->GetLogicType()==fSETUP)   {
            StartComment(tss);
            tss << "//the setup routine runs once when you press reset\n";
            EndComment(tss);
            node->WriteSourceUserGuts(tss);
            break; // only one setup
        }
    }
    StartComment(tss);
    tss << "// the loop routine runs over and over again forever\n";
    EndComment(tss);
    tss << "void loop() {\n";
    foreach (Node *node, nodes) {
        node->WriteHeader(tsh);

        if (node->GetLogicType()==fOUT)

            tss << "  " << node->getName() << ";\n";
    }
    tss << "}\n\n\n";
    StartComment(tss);
    tss<< "These are the routines written by the Fuzzybuilder\n";
    EndComment(tss);
    foreach (Node *node, nodes) {
        if (node->GetLogicType()==fOUT)
            node->WriteSourceUserGuts(tss);
    }
    bool notice = true;

    foreach (Node *node, nodes) {
        if (!node->getHeaderBeenWritten())
            node->WriteHeader(tsh);
        if (!node->getSourceBeenWritten())
        {
            if (notice) {
                notice = false;
                tss << "\n\n";
                StartComment(tss);
                tss << "// the following Logic Block(s) are not used\n//But are available for use with the FuzzyBuilder\n";
                EndComment(tss);
            }
            node->WriteSourceUserGuts(tss);
        }
    }
}
//! [7]

void GraphWidget::shuffle()
{
    foreach (QGraphicsItem *item, scene()->items()) {
        if (qgraphicsitem_cast<Node *>(item))
            item->setPos(-150 + qrand() % 300, -150 + qrand() % 300);
    }
}

void GraphWidget::zoomIn()
{
    scaleView(qreal(1.2));
}

void GraphWidget::zoomOut()
{
    scaleView(1 / qreal(1.2));
}

