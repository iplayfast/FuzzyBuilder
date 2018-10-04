#include "edge.h"
#include "node.h"
#include "graphwidget.h"
#include "mainwindow.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <qevent.h>
#include <qnamespace.h>
#include <QDebug>

//! [0]

QString Node::getINTEXT() const
{
    return INTEXT;
}

void Node::setINTEXT(const QString &value)
{
    INTEXT = value;
}
Node::Node(GraphWidget *graphWidget)
    : graph(graphWidget)
{
    QGraphicsScene *scene = graphWidget->scene();
    scene->addItem(this);
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
    selected = false;
    INTEXT = "";
    QRect exposedRect(graphWidget->mapToScene(0,0).toPoint(),graphWidget->viewport()->rect().size());
    setPos(exposedRect.width()/2,exposedRect.height() / 2);

}
//! [0]

//! [1]
void Node::addEdge(Edge *edge)
{
    edgeList << edge;
    edge->adjust();
}

QList<Edge *> Node::edges() const
{
    return edgeList;
}
QString Node::getName() const
{
    return Name;
}

void Node::drawShape(QPainter *p,QRectF &r)
{
    p->drawEllipse(r);
}

QColor Node::getGradient(int zero_one)
{
    if (zero_one)
        return QColor(Qt::green);
    return QColor(Qt::darkGreen);
}

QRectF Node::boundingRect() const
{
int width = 20;
int height = 20;
qreal adjust = 2;
return QRectF( -width - adjust, -height - adjust, 2 * width + adjust, 2 * height + adjust);
}


void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget */*widget*/)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::darkGray);
    QRectF r =  boundingRect();
    r.adjust(-3,-3,0,0);
    drawShape(painter,r);
//    painter->drawEllipse(r);
    r.adjust(3,3,0,0);
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
            gradient.setColorAt(1,getGradient(1));
            gradient.setColorAt(0,getGradient(0));
        }
    }

    painter->setBrush(gradient);



    painter->setPen(QPen(Qt::black, 0));
    QString v;
        v.sprintf("\n%05.5f",getCurrent());
    QString NameV(getName());
    NameV += v;
    {
        drawShape(painter,r);
    r.adjust(-15,-15,15,15);
        painter->drawText(r,Qt::AlignHCenter,NameV);
//        painter->drawText(r.bottomLeft(),NameV);
    }
}
void Node::setName(const QString &value)
{
    Name = value;
}


void Node::calculateForces()
{

    if (!scene() || scene()->mouseGrabberItem() == this || selected) {
        if (!scene()) return;
        if (pos()==newPos)
            return;
        QPointF Diff = pos() - newPos;
        Diff.setY(-Diff.y());
        Diff.setX(-Diff.x());
            foreach (QGraphicsItem *item, scene()->items()) {
                Node *node = qgraphicsitem_cast<Node *>(item);
                if (!node)
                    continue;
                    if (node->selected){
                        node->setPos(newPos - Diff);
//                        QString s;
//                        s.sprintf("%d %d",node->pos().rx(),node->pos().ry());
//                                node->Name = s;
                    }
            }

        return;
    }

    if (wp->getFrozen())
            return;


    // Sum up all forces pushing this item away
    qreal xvel = 0;
    qreal yvel = 0;
    foreach (QGraphicsItem *item, scene()->items()) {
        Node *node = qgraphicsitem_cast<Node *>(item);
        if (!node)
            continue;
        QPointF vec = mapToItem(node, 0, 0);
        qreal dx = vec.x();
        qreal dy = vec.y();
        double l = 2.0 * (dx * dx + dy * dy);
        if (l > 0) {
            xvel += (dx * 150.0) / l;
            yvel += (dy * 150.0) / l;
        }
    }
//! [3]
QRectF sceneRect = scene()->sceneRect();
QPointF vec = pos();
int w2 = sceneRect.width()/2;
    switch(this->GetLogicType()){
    case fIN:
        xvel -=50;
        break;
    case fOUT:
        xvel += 50;
        break;
    case fFUZZY:
        yvel += 5;
        xvel += +(vec.rx() - w2) / 10;
        break;
    case fAND:
    case fOR:
        yvel -=5;
        xvel += +(vec.rx() - w2) / 10;
        break;
    case fPID:
    case fTIMER:
        break;
    }

//! [4]
    // Now subtract all forces pulling items together
    double weight = (edgeList.size() + 1) * 10;
    foreach (Edge *edge, edgeList) {
        QPointF vec;
        if (edge->sourceNode() == this)
            vec = mapToItem(edge->destNode(), 0, 0);
        else
            vec = mapToItem(edge->sourceNode(), 0, 0);
        xvel -= vec.x() / weight;
        yvel -= vec.y() / weight;
    }
//! [4]
    if (this->fType==fIN)
        xvel -= 50;
    if (this->fType==fOUT)
        xvel += 50;
//! [5]
    if (qAbs(xvel) < 0.1 && qAbs(yvel) < 0.1)
        xvel = yvel = 0;
//! [5]

//! [6]
    //QRectF sceneRect = scene()->sceneRect();
    QRectF r = boundingRect();
    QPointF newPos1;
    newPos1 = pos() + QPointF(xvel, yvel);
    newPos1.setX(qMin(qMax(newPos.x(), sceneRect.left() + r.width()), sceneRect.right() - r.width()));
    newPos1.setY(qMin(qMax(newPos.y(), sceneRect.top() + r.height()), sceneRect.bottom() - r.height()));
    setNewPos(newPos1);
}
//! [6]

//! [7]
bool Node::advance()
{
    if (newPos == pos())
        return false;

    setPos(newPos);
    return true;
}
//! [7]

//! [8]
/*QRectF Node::boundingRect() const
{
    int width=0;
    int height = 0;
    switch(this->fType)
    {
        case fIN:
            width = 40;
            height = 20;
            break;
    case fOUT:
            width = 40;
            height = 20;
            break;
    case fFUZZY:
            width = 40;
            height = 40;
            break;
    case fAND:
    case fOR:
    default:
            width = 20;
            height = 20;
            break;
    }

    qreal adjust = 2;

    return QRectF( -width - adjust, -height - adjust, 2 * width + adjust, 2 * height + adjust);
}
//! [8]
*/
//! [9]
QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addEllipse(boundingRect());//-10, -10, 20, 20);
    return path;
}
//! [9]

//! [10]
/*void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::darkGray);
QRectF r =  boundingRect();
    r.adjust(-3,-3,0,0);
    if (fType==fFUZZY)
        painter->drawRect(r);
    else
        painter->drawEllipse(r);
    r.adjust(3,3,0,0);
    QRadialGradient gradient(r.center(),r.height());
    if (option->state & QStyle::State_Sunken) {
        gradient.setCenter(3, 3);
        gradient.setFocalPoint(3, 3);
        if (!selected) {
            gradient.setColorAt(1, QColor(Qt::yellow).light(120));
            gradient.setColorAt(0, QColor(Qt::darkYellow).light(120));
        }
        else {
            gradient.setColorAt(1, QColor(Qt::green).light(120));
            gradient.setColorAt(0, QColor(Qt::darkGreen).light(120));
        }
    } else {
        if (!selected) {
            gradient.setColorAt(0, Qt::yellow);
            gradient.setColorAt(1, Qt::darkYellow);
        }
        else {
            gradient.setColorAt(1, QColor(Qt::green));
            gradient.setColorAt(0, QColor(Qt::darkGreen));
        }
    }

    painter->setBrush(gradient);



    painter->setPen(QPen(Qt::black, 0));
    QString v;
        v.sprintf(" %05.5f",Current);
    QString NameV(Name);
    NameV += v;
    switch(this->fType)
    {
    case fIN:
    {
        painter->drawEllipse( r);//-5, -10, 20, 20);
        painter->drawText(r,Qt::AlignHCenter,"In\n"+NameV);
    }
        break;
    case fOUT:
    {
        painter->drawEllipse( r);//-5, -10, 20, 20);
        painter->drawText(r,Qt::AlignHCenter,"Out\n"+NameV);
    }
        break;
    case fFUZZY:
        painter->drawRect(r);
        painter->drawText(r,Qt::AlignHCenter,"Fuzzy\n"+NameV);
    break;
    //case fAND:
    //{
//        painter->drawEllipse( r);//-5, -10, 20, 20);
//        painter->drawText(r,Qt::AlignHCenter,"AND\n"+NameV);
//    }
//        break;
    case fOR:
    {
        painter->drawEllipse( r);//-5, -10, 20, 20);
        painter->drawText(r,Qt::AlignHCenter,"OR\n"+NameV);
    }
        break;
    case fPID:
    {
        painter->drawRect(r);
        painter->drawText(r,Qt::AlignHCenter,"PID\n"+NameV);
    }
        break;
    }

}
*/
void Node::setPos(qreal ax, qreal ay)
{
    setPos(QPointF(ax,ay));
}

void Node::setPos(QPointF p)
{
    setNewPos(p);
    QGraphicsItem::setPos(p);
}


/*bool Node::AllowAttach(Node *)
{
    switch(fType)
    {
    case fIN:
        return false;   //inputs are not attachies
    case fOUT: // outputs can have one input
    case fFUZZY: // only one input
        return (edgeList.count()<1);


    case fAND:
    case fOR:
            return true;
    case fPID: return false; // needs some thought
    }
    return false;
}
*/
//! [11]
QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        foreach (Edge *edge, edgeList)
            edge->adjust();
        graph->itemMoved();
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}

// returns true if an edge was deleted between this and node
bool Node::deleteEdge(Node *node)
{
    bool deleted = false;
    foreach (Edge *edge, edgeList)
    {
        if (edge->sourceNode()==node) {
            node->edges().removeOne(edge);
            edges().removeOne(edge);
            node->edgeList.removeOne(edge);
            edgeList.removeOne(edge);
            scene()->removeItem(edge);
            deleted = true;
            //break;
        }
    }

    return deleted;
}

// will add or remove edges from selected nodes to this one, or if DeleteAll will delete any edges from any nodes to this one.
void Node::ModifyEdges(bool DeleteAll)
{
    foreach (QGraphicsItem *item, scene()->items()) {
        Node *node = qgraphicsitem_cast<Node *>(item);
        if (!node || node==this)
            continue;
        if (DeleteAll)
                deleteEdge(node);
        else    {
            if (node->selected) {
                if (!deleteEdge(node) && AllowAttach(node)) {
                    this->scene()->addItem(new Edge(node,this));
                }
            }
        }
    }
}
QPointF Node::getNewPos() const
{
    return newPos;
}

void Node::setNewPos(const QPointF &value)
{
    newPos = value;
}


void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Qt::MouseButton b = event->button();
    if (b & Qt::RightButton) {
        ModifyEdges(false);
        // if at this point no other node had an edge selected so this is first one
        QGraphicsItem::mousePressEvent(event);
        return;
    }
    if (graph->GetShift()) selected = true;
    else {
        foreach (QGraphicsItem *item, scene()->items()) {
            Node *node = qgraphicsitem_cast<Node *>(item);
            if (!node)
                continue;
            node->selected = (node==this);
            node->update(node->boundingRect());
        }
    }
    wp->DisplayNode(this);
    event->setAccepted(true);
    update();
    QGraphicsItem::mousePressEvent(event);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

void Node::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (selected) calculateForces();
    QGraphicsItem::mouseMoveEvent(event);
}

QVariant Node::headerData(int section, Qt::Orientation orientation, int role) const
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

void Node::editCompleted(const QString &)
{
    //
}

Qt::ItemFlags Node::flags(const QModelIndex &/*index*/) const
{
    return Qt::ItemIsSelectable |  Qt::ItemIsEditable | Qt::ItemIsEnabled ;
}
bool Node::getBeenWritten() const
{
    return BeenWritten;
}

void Node::setBeenWritten(bool value)
{
    BeenWritten = value;
}


double Node::getActiveValue() const
{
    return ActiveValue;
}

void Node::setActiveValue(double value)
{
    ActiveValue = value;
}

double Node::getIOMin() const
{
    return IOMin;
}

void Node::setIOMin(double value)
{
    IOMin = value;
}

double Node::getIOMax() const
{
    return IOMax;
}

void Node::setIOMax(double value)
{
    IOMax = value;
}

bool Node::getSelected() const
{
    return selected;
}

void Node::setSelected(bool value)
{
    selected = value;
}

/*double Node::Simulate()
{
double v;
    switch(this->fType)
    {
    case fIN:
        v = (InValue - IOMin) / (IOMax - IOMin); // normalize
        Current = v;
        return Current;
    case fOUT:
        Current = 0.0;
        foreach (Edge *edge, edgeList)
        {
            if (edge->sourceNode()!=this)
            {
                v = edge->sourceNode()->Simulate();
                Current = (IOMax - IOMin) * v + IOMin;
                return Current;
            }
        }
        return Current;

    case fFUZZY:
        Current = 0.0;
        foreach (Edge *edge, edgeList)
        {
            if (edge->sourceNode()!=this)
            {
                v = edge->sourceNode()->Simulate();
                Current = fuzzy.Value(v);
                return Current;
            }
        }
        return Current;
    case fAND:
        Current = 1.0;
        foreach (Edge *edge, edgeList)
        {
            if (edge->sourceNode()!=this)
            {
                double v = edge->sourceNode()->Simulate();
                if (Current>v) Current = v;
            }
        }
        return Current;
    case fOR:
        Current = 0.0;
        foreach (Edge *edge, edgeList)
        {
            if (edge->sourceNode()!=this)
            {
                double v = edge->sourceNode()->Simulate();
                if (Current<v) Current = v;
            }
        }
        return Current;
    case fPID:// todo
        Current= 0;
        return Current;
    default:
        return Current;
    }

}
*/
double Node::getCurrent() const
{
    return Current;
}

void Node::setCurrent(double value)
{
    Current = value;
}



//! [12]
