#include "mainwindow.h"
#include "fuzzynode.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <qevent.h>
#include <qnamespace.h>
#include <QDebug>

FuzzyNode::FuzzyNode(GraphWidget *graphWidget) : Node(graphWidget)
{
    setWidth(40);
    setHeight(40);
    QRect exposedRect(graphWidget->mapToScene(0,0).toPoint(),graphWidget->viewport()->rect().size());
    setPos(exposedRect.width()/2,exposedRect.height() / 2);
    if (!FindNewVertPosition(-1))
        FindNewVertPosition(1);


}

QRectF FuzzyNode::boundingRect() const
{
    int width = getWidth();
    int height = getHeight();
qreal adjust = 2;
return QRectF( -width - adjust, -height - adjust, 2 * width + adjust, 2 * height + adjust);
}

bool FuzzyNode::AllowAttach(Node *) const
{
    return edgeList.count()<1 || edgeList.first()->getDest()!=this;
}


void FuzzyNode::WriteHeader(QTextStream &h)
{
    if (getHeaderBeenWritten()) return;
    setHeaderBeenWritten(true);
    h << "#include <Fuzzy.h>\n";
    h << "double " << getName() << "(void);\n";
    h << "struct TFuzzy" << getName() <<" {\n    int Count;\n    TFuzzyXY Data[" << fuzzy.Count() << "];\n};\n\n";
}

void FuzzyNode::WriteNodeInfo(QTextStream &s)
{
    QString ps; ps.sprintf("!!%f!!%f",pos().rx(),pos().ry());
    StartComment(s);
    s << "//!!fFuzzy!!" << getName() << ps;
    for(int i=0;i<fuzzy.Count();i++)
        s << "!!" << fuzzy.GetItemc(i)->x << "!!" << fuzzy.GetItemc(i)->y;
    EndComment(s);
    foreach (Edge *edge, edgeList)
    {
        if (edge->getSource()!=this)
        {
            edge->getSource()->WriteNodeInfo(s);
        }
    }

}

void FuzzyNode::FunctionData(QString &Return, QString &Parameters, QString &FunctionReturn)
{
    Return = "double ";
    Parameters = "()";
    FunctionReturn = "return Value;";
}





void FuzzyNode::WriteSourcePlainGuts(QTextStream &s)
{
    s << "\nstruct TFuzzy" << getName() << " = {" << fuzzy.Count() << "{\n";
    for(int i=0;i<fuzzy.Count();i++)
    {
        s << "\n   ,{" << fuzzy.GetItemc(i)->x << "," << fuzzy.GetItemc(i)->y << "}";
    }
    s << "}};\n";

    foreach (Edge *edge, edgeList)
    {
        if (edge->getSource()!=this)
        {
            //edge->getSource()->WriteSourceUserGuts(s);
            s << "   double value = Value((const struct TFuzzy *)&TFuzzy" << edge->getSource()->getName() << ");\n";
            return; // only one incoming edge ever
        }
    }
    s << "// unused fuzzy logic node\n";

}

void FuzzyNode::setIOMin(double value)
{
    Node::setIOMin(value);
    double lIOMax = fuzzy.Value(value);
        if (lIOMax==-1)  // no value found
            lIOMax = 0; // false
        Node::setIOMax(lIOMax);
}



double FuzzyNode::Simulate()
{
double    Current = 0.0;
    foreach (Edge *edge, edgeList)
    {
        if (edge->getSource()!=this)
        {
            setInValue(edge->getSource()->Simulate());
            setCurrent(Current = fuzzy.Value(getInValue()));
            this->update();
            return Current;
        }
    }
    setCurrent(Current);
    update();
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

QVariant FuzzyNode::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal) {
            switch (section)
            {
            case 0:
                return QString("Cause");
            case 1:
                return QString("Effect");
            }
        }
    }
    return QVariant();
}
bool FuzzyNode::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole)
       {
        QString s = value.toString();
        double d = value.toDouble();
            if (d>1.0 || d<0.0) {
                return false;
            }
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
    wp->UpdateTableAndGraph(this);
       return true;

}

QPainterPath FuzzyNode::shape() const
{
    QPainterPath a;
    a.addRect(boundingRect());
    return a;
}

Qt::ItemFlags FuzzyNode::flags(const QModelIndex &/*index*/) const
{
   return Qt::ItemIsSelectable |  Qt::ItemIsEditable | Qt::ItemIsEnabled ;
}
