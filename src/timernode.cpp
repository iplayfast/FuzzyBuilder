

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



void TimerNode::drawShape(QPainter *p, QRectF &r)
{
//   p->drawEllipse(r);
//   p->drawPie(r,5,10);
   QPainterPath path,epath;
    // make face
    epath.addEllipse(r);
    // make hands
    for(int i=-1;i<2;i++)
        for(int j=-1;j<2;j++)   {
            epath.moveTo(i,j);
            epath.lineTo(-r.width()/10,-r.height()/10);
            epath.moveTo(i,j);
            epath.lineTo(r.width()/6,-r.height()/6);
        }
    QPainterPath pp =  epath.subtracted(path);
    p->drawPath(pp);
}

TimerNode::TimerNode(GraphWidget *graphWidget) : Node(graphWidget)
{
    setIOMax(255);
}

bool TimerNode::AllowAttach(Node *) const
{
    return true;
}

void TimerNode::WriteSource(QTextStream &h, QTextStream &s)
{
    if (getBeenWritten()) return;
    foreach (Edge *edge, edgeList)
    {
        if (edge->sourceNode()!=this)
            edge->sourceNode()->WriteSource(h,s);
    }

    QString ps; ps.sprintf("!!%f!!%f!!%f\n",pos().rx(),pos().ry(),getIOMin());
    h << "//!!fTimer!!" << getName() <<  ps;
    s << "void " << getName() << "()\n{";
    QString ss;
        ss.sprintf("%u",(unsigned int)this->getIOMin());

    s << "\\\\ timer checks the input value every " << ss << " milliseconds and always returns last value obtained\n";
    s << "static double Current=0;\n";
    s << "static unsigned long milliseconds=0;\nunsigned long ms;\n";
    s << "    ms = millis(); // arduino specific\n";
    s << "    if (ms>=milliseconds ) {\n";
    s << "        milliseconds = ms + " << ss << " - (ms - milliseconds);\n";
    foreach (Edge *edge, edgeList) {
         if (edge->sourceNode()!=this)
         {
            s << "        return Current = " << edge->sourceNode()->getName() << "();\n";
            s << "    }\n";
            return; // only one edge in a timer
        }
    }

    s << "    return Currrent;\n";
    s << "    }\n";
    return;
}

double TimerNode::Simulate()
{
    foreach(Edge *edge,edgeList)
    {
        if (edge->sourceNode()!=this)
                return edge->sourceNode()->Simulate();
    }
    return 0;
}

