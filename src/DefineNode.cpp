
#include "DefineNode.h"

DefineNode::DefineNode(GraphWidget *graphWidget) : Node(graphWidget)
{

    setIOMax(1023);
    setIOMin(0);
    QRect exposedRect(graphWidget->mapToScene(0,0).toPoint(),graphWidget->viewport()->rect().size());
    setPos(0,exposedRect.height() / 2);
    if (!FindNewVertPosition(-1))
        FindNewVertPosition(1);

    this->setCurrent(0);
    this->setActive(0);
    this->setInValue(0);
    Regenerate();

}

QRectF DefineNode::boundingRect() const
{
    int width = getWidth()/4;
    int height = getHeight()/4;
    qreal adjust = 2;
    return QRectF( -width - adjust, -height - adjust, 2 * width + adjust, 2 * height + adjust);
}

QPainterPath DefineNode::shape() const
{
    QPainterPath path,epath;
    QRectF r= boundingRect();

    epath.addEllipse(r);
    path.moveTo(r.width()/2,0);
    path.lineTo(r.width()/2,r.height());
    path.moveTo(0,r.height()/2);
    path.lineTo(r.width(),r.height()/2);
    //    path.addRect(r);
    return epath.subtracted(path);
}

QString DefineNode::Regenerate() const
{
    QString s;
    s = "#define ";
    s += getName();
    s += " ";
    QString v;
    v.sprintf("%d",(int)getIOMin());
    s += v;
    return s;
}

void DefineNode::WriteNodeInfo(QTextStream &ts) const
{
    QString ps; ps.sprintf("!!%f!!%f!!%f\n",pos().rx(),pos().ry(),getIOMin());
    ts << "\n";
    ts << "//!!fDefine!!" << getName() <<  ps;
    Node::WriteNodeInfo(ts);

}

void DefineNode::FunctionData(QString &Return, QString &Parameters, QString &FunctionReturn, bool &HasBrackets) const
{
    Return = "// global code ";
    Parameters = "";
    FunctionReturn = "";
    HasBrackets = false;
}

void DefineNode::WriteSourcePlainGuts(QTextStream &ts) const
{
    if (getUserGuts()=="")
        ts << Regenerate();
    else ts << getUserGuts();
}

QString DefineNode::getIOMinText() const
{
    QString ss;
    ss.sprintf("%05d",(int)getIOMin());
    return ss;
}


