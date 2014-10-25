

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
    setPos(exposedRect.width()/2,exposedRect.height() / 2);
    if (!FindNewVertPosition(-1))
        FindNewVertPosition(1);
    setIOMax(255);

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

void TimerNode::WriteHeader(QTextStream &h) const
{
    h << "void " << getName() << "(void);\n";
}

void TimerNode::WriteNodeInfo(QTextStream &s) const
{
    QString ps; ps.sprintf("!!%f!!%f!!%f\n",pos().rx(),pos().ry(),getIOMin());
    s << "//!!fTimer!!" << getName() <<  ps;
    Node::WriteNodeInfo(s);
}

int TimerNode::MaxOfMin() const
{
    return 256 * this->getMinScale();
}

QString TimerNode::GetValueText() const
{
    QString v = Node::GetValueText();
    QString v1;
        v1.sprintf("\n(%d)",(int)round(this->getActiveValue()));
        v1 += v;
        return v1;
}

void TimerNode::FunctionData(QString &Return, QString &Parameters, QString &FunctionReturn, bool &HasBrackets) const
{
 Return = "void ";
 Parameters = "()";
 FunctionReturn = "return Current;";
 HasBrackets = true;
}

void TimerNode::WriteSourcePlainGuts(QTextStream &s) const
{
    if (getUserGuts()=="")
        s << Regenerate();
    else s << getUserGuts();
}

QString TimerNode::Regenerate() const
{
    QString ss;
        ss.sprintf("%u",(unsigned long)this->getIOMin());

    QString s = "static double Current=-1.0;// first time call flag value\n";
    s += "static unsigned long milliseconds;\nunsigned long ms;\n";
    s += "    ms = millis(); // arduino specific\n";
    s += "    if (ms>=milliseconds || Current==-1.0) {\n";
    s += "        milliseconds = ms + "; s += ss; s += " - (ms - milliseconds);\n";
    foreach (Edge *edge, edgeList) {
                if (edge->getSource()!=this)
                {
                    s += "        Current = "; s+= edge->getSource()->getName(); s+= "();\n";
                    s += "    }\n";
                    return s;
                }
    }
    // if it got here input has not been setup

    s += "    currrent = //Need an input connected for valid results\n";
    s += "    }\n";
    return s;
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
    return epath.subtracted(path);
}
