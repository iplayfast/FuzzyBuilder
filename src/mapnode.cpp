#include "mapnode.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <qevent.h>
#include <qnamespace.h>
#include <QDebug>

mapnode::mapnode(GraphWidget *graphWidget):Node(graphWidget)
{

}

bool mapnode::AllowAttach(Node *) const
{
    return true;
}

void mapnode::WriteSource(QTextStream &h, QTextStream &s)
{
    if (getBeenWritten()) return;
    setBeenWritten(true);
    QString ss;
        ss.sprintf("%05.5f",ActiveValue);


    foreach (Edge *edge, edgeList)
    {
        if (edge->sourceNode()!=this)
            edge->sourceNode()->WriteSource(h,s);
    }
    QString ps; ps.sprintf("!!%f!!%f\n",pos().rx(),pos().ry());
    h << "//!!fMap!!" << getName() << "!!" <<  ps;
    h << "double " << getName() << "(void);\n";
    s << "\ndouble " << getName() << "(void)\n{\n";
    s << "double  Result = 0.0;\n//all inputs to the map are in range 0..1\n";
    int count=1;
        foreach (Edge *edge, edgeList)
        {
            if (edge->sourceNode()!=this)
            {
                s << "   Result += " <<
                        edge->sourceNode()->getName() << "() * " <<
                     count << ";\n";
                count +=IOMax;
            }
        }
        count-=IOMax;
        s << "    Result = Result / " << count;
        s << "return Result;\n}\n";
        foreach (Edge *edge, edgeList)
            if (edge->sourceNode()!=this)
                edge->WriteSource(h);

}

double mapnode::Simulate()
{
double Result=0.0;
int count = 1;
    foreach(Edge *edge,edgeList)
    {
        if (edge->sourceNode()!=this) {
            double v=edge->sourceNode()->Simulate();
            Result += v *count;
            count +=IOMax;
        }
    }
    count-=IOMax;
    Result = Result / count;
    setCurrent(Result);
    return Result;
}
