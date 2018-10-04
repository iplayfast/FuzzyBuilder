#include "fuzzynode.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <qevent.h>
#include <qnamespace.h>
#include <QDebug>

void FuzzyNode::drawShape(QPainter *p, QRectF &r)
{
    p->drawRect(r);
}


FuzzyNode::FuzzyNode(GraphWidget *graphWidget) : Node(graphWidget)
{

}

QRectF FuzzyNode::boundingRect() const
{
int width = 40;
int height = 40;
qreal adjust = 2;
return QRectF( -width - adjust, -height - adjust, 2 * width + adjust, 2 * height + adjust);
}

bool FuzzyNode::AllowAttach(Node *) const
{
    return edgeList.count()<1 || edgeList.first()->destNode()!=this;
}



void FuzzyNode::WriteSource(QTextStream &h,QTextStream &s)
{
    if (getBeenWritten()) return;
    setBeenWritten(true);

    foreach (Edge *edge, edgeList)
    {
        if (edge->sourceNode()!=this)
        {
            edge->sourceNode()->WriteSource(h,s);
            QString ps; ps.sprintf("!!%f!!%f",pos().rx(),pos().ry());
            h << "//!!fFuzzy!!" << getName() << ps;

            for(int i=0;i<fuzzy.Count();i++)
            {
                h << "!!" << fuzzy.GetItemc(i)->x << "!!" << fuzzy.GetItemc(i)->y;
            }
            h << "\n";

            h << "double " << getName() << "(void);\n";
            s << "struct TFuzzy" << getName() << "\nint DataLength\nstruct TFuzzyXY Data["<< this->fuzzy.Count() << "];\n};\n";
            s << "double " << getName() << "(void)\n{\n";
            s << "struct TFuzzy" << getName() << " = {" << fuzzy.Count();
            for(int i=0;i<fuzzy.Count();i++)
            {
                s << "\n   ,{" << fuzzy.GetItemc(i)->x << "," << fuzzy.GetItemc(i)->y << "}";
            }
            s << "};\n   return Value((const struct TFuzzy *) &Data," << edge->sourceNode()->getName()<<"());\n}\n\n";
            foreach (Edge *edge, edgeList)
                if (edge->sourceNode()!=this)
                    edge->WriteSource(h);

            return;
        }
    }
    foreach (Edge *edge, edgeList)
        if (edge->sourceNode()!=this)
            edge->WriteSource(h);
    return;
}

double FuzzyNode::Simulate()
{
double    Current = 0.0;
    foreach (Edge *edge, edgeList)
    {
        if (edge->sourceNode()!=this)
        {
            double v = edge->sourceNode()->Simulate();
            setCurrent(Current = fuzzy.Value(v));
            return Current;
        }
    }
    setCurrent(Current);
    return Current;
}

int FuzzyNode::rowCount(const QModelIndex &/*parent*/) const
{
    return fuzzy.Count(); // add 1 for top row labels
}

int FuzzyNode::columnCount(const QModelIndex &/*parent*/) const
{
    return 2;
}
QVariant FuzzyNode::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();
    //qDebug() << QString("row %1, col%2, role %3") .arg(row).arg(col).arg(role);

    switch(role) {
    case Qt::DisplayRole:
        {
            const TFuzzyXY *xy= fuzzy.GetItemc(row);
            QString text;
            text.sprintf("%0.4f",(col==0 ? xy->x : xy->y));
            return text;
        }
        break;
    case Qt::FontRole:
        break;
    case Qt::BackgroundColorRole:
        // QBrush bg(Qt::red);
        // return bg;
        break;
    /*case Qt::TextAlignmentRole:

             if (row == 1 && col == 1) //change text alignment only for cell(1,1)
             {
                 return Qt::AlignRight + Qt::AlignVCenter;
             }
             break;
         case Qt::CheckStateRole:

             if (row == 1 && col == 0) //add a checkbox to cell(1,0)
             {
                 return Qt::Checked;
             }
      */
    }
    return QVariant();
}

bool FuzzyNode::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole)
       {
        QString s = value.toString();
        double d = value.toDouble();
            int causeeffect = index.column();
            int findex = index.row();
            double oldx,oldy;
            TFuzzyXY *oldxy = fuzzy.GetItem(findex);
                oldx = fuzzy.ValueAt(findex);
                oldy = fuzzy.IndexAt(findex);
                if (causeeffect==0){
                    oldx = d;
                    this->fuzzy.DeleteItem(findex);
                    this->fuzzy.TFuzzyAddPoint(oldx,oldy);
                }
                else {
                    oldxy->y = d;
                }
            //wp->DisplayNode(this);
       }
       return true;

}
