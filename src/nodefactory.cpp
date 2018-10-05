#include "nodefactory.h"
#include "graphwidget.h"
#include <QStringList>
NodeFactory::NodeFactory()
{
}

Node *NodeFactory::Create(GraphWidget *graphWidget, LOGICTYPE lt)
{
    switch(lt) {
    case fIN:
        return new InNode(graphWidget);
    case fOUT:
        return new OutNode(graphWidget);
    case fFUZZY:
        return new FuzzyNode(graphWidget);
    case fAND:
        return new AndNode(graphWidget);
    case fOR:
        return new OrNode(graphWidget);
    case fPID:
        return new PidNode(graphWidget);
    case fTIMER:
        return new TimerNode(graphWidget);
    case fMAP:
        return new mapnode(graphWidget);
    default:
        throw "Bad LogicType";
    }
}


void NodeFactory::ReadSource(GraphWidget *graphWidget,QTextStream &s)
{
QString Line;
    while(!s.atEnd())   {
        Line = s.readLine();
        if (Line.startsWith("//!!")) {
            QStringList Stringlist = Line.split("!!");
            LOGICTYPE lt;
            if (Stringlist[1]=="fIn") lt = fIN;
            if (Stringlist[1]=="fOut") lt = fOUT;
            if (Stringlist[1]=="fOr") lt = fOR;
            if (Stringlist[1]=="fAnd") lt = fAND;
            if (Stringlist[1]=="fMap") lt = fMAP;
            if (Stringlist[1]=="fEdge")  {
                Node *source,*dest;
                source = graphWidget->FindNode(Stringlist[2]);
                dest = graphWidget->FindNode(Stringlist[3]);
                if (source && dest)
                        graphWidget->scene()->addItem(new Edge(source,dest));
                continue;
            }
            if (Stringlist[1]=="fFuzzy") lt = fFUZZY;
            if (Stringlist[1]=="fPid") lt = fPID;

            switch(lt)
            {
            case fIN:
            {
                int lc = Stringlist.count();
                InNode *n = new InNode(graphWidget);
                n->setName(Stringlist[2]);
                graphWidget->scene()->addItem(n);
                n->setPos(Stringlist[3].toDouble(),Stringlist[4].toDouble());
                if (lc>5) {
                n->setIOMin(Stringlist[5].toDouble());
                n->setIOMax(Stringlist[6].toDouble());
            }
            }

                break;
            case fOUT:
            {
                OutNode *n = new OutNode(graphWidget);
                n->setName(Stringlist[2]);
                graphWidget->scene()->addItem(n);
                n->setPos(Stringlist[3].toDouble(),Stringlist[4].toDouble());
            }
                break;
            case fFUZZY:
            {
             FuzzyNode *n = new FuzzyNode(graphWidget);
                n->setName(Stringlist[2]);
                for(int i=5;i<Stringlist.count();i+=2)
                {
                    double cause,effect;

                    cause = Stringlist[i].toDouble();
                    effect = Stringlist[i+1].toDouble();
                    n->fuzzy.TFuzzyAddPoint(cause,effect);
                }
                graphWidget->scene()->addItem(n);
                n->setPos(Stringlist[3].toDouble(),Stringlist[4].toDouble());
            }

                break;
            case fAND:
            {
             AndNode *n = new AndNode(graphWidget);
                n->setName(Stringlist[2]);
                n->setActiveValue(Stringlist[3].toDouble());
                graphWidget->scene()->addItem(n);
                n->setPos(Stringlist[4].toDouble(),Stringlist[4].toDouble());
            }
                break;
            case fOR:
            {
             OrNode *n = new OrNode(graphWidget);
             n->setName(Stringlist[2]);
             n->setActiveValue(Stringlist[3].toDouble());
             graphWidget->scene()->addItem(n);
             n->setPos(Stringlist[4].toDouble(),Stringlist[4].toDouble());
            }
                break;
            case fPID:
            {
            PidNode *n = new PidNode(graphWidget);
                n->setName(Stringlist[2]);
                graphWidget->scene()->addItem(n);
             n->setPos(Stringlist[4].toDouble(),Stringlist[4].toDouble());
            }
                break;
            case fMAP:
            {
               mapnode *n = new mapnode(graphWidget);
                n->setName(Stringlist[2]);
                n->setActiveValue(Stringlist[3].toDouble());
                graphWidget->scene()->addItem(n);
             n->setPos(Stringlist[4].toDouble(),Stringlist[4].toDouble());
            }
                break;
            default:
                throw "Bad LogicType";
            }
        }
    }
}
