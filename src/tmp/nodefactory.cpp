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
    case fNOT:
        return new NotNode(graphWidget);
    case fPID:
        return new PidNode(graphWidget);
    case fTIMER:
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
            QStringList list1 = Line.split("!!");
            LOGICTYPE lt;
            if (list1[1]=="fIn") lt = fIN;
            if (list1[1]=="fOut") lt = fOUT;
            if (list1[1]=="fOr") lt = fOR;
            if (list1[1]=="fAnd") lt = fAND;
            if (list1[1]=="fNOT") lt = fNOT;

            if (list1[1]=="fEdge")  {
                Node *source,*dest;
                source = graphWidget->FindNode(list1[2]);
                dest = graphWidget->FindNode(list1[3]);
                if (source && dest)
                        graphWidget->scene()->addItem(new Edge(source,dest));
                continue;
            }
            if (list1[1]=="fFuzzy") lt = fFUZZY;
            if (list1[1]=="fPid") lt = fPID;

            switch(lt)
            {
            case fIN:
            {
                InNode *n = new InNode(graphWidget);
                n->setName(list1[2]);                
                n->setPos(list1[3].toDouble(),list1[4].toDouble());

            }
                break;
            case fOUT:
            {
                OutNode *n = new OutNode(graphWidget);
                n->setName(list1[2]);                
                n->setPos(list1[3].toDouble(),list1[4].toDouble());
            }
                break;
            case fFUZZY:
            {
             FuzzyNode *n = new FuzzyNode(graphWidget);
                n->setName(list1[2]);
                for(int i=5;i<list1.count();i+=2)
                {
                    double cause,effect;

                    cause = list1[i].toDouble();
                    effect = list1[i+1].toDouble();
                    n->fuzzy.TFuzzyAddPoint(cause,effect);
                }                
                n->setPos(list1[3].toDouble(),list1[4].toDouble());
            }

                break;
            case fAND:
            {
             AndNode *n = new AndNode(graphWidget);
                n->setName(list1[2]);
                n->setActiveValue(list1[3].toDouble());                
                n->setPos(list1[4].toDouble(),list1[5].toDouble());
            }
                break;
            case fOR:
            {
             OrNode *n = new OrNode(graphWidget);
             n->setName(list1[2]);
             n->setActiveValue(list1[3].toDouble());             
             n->setPos(list1[4].toDouble(),list1[5].toDouble());
            }
                break;
            case fNOT:
            {
                NotNode *n = new NotNode(graphWidget);
                n->setName(list1[2]);                
                n->setPos(list1[3].toDouble(),list1[4].toDouble());
            }
                break;
            case fPID:
            {
            PidNode *n = new PidNode(graphWidget);
                n->setName(list1[2]);                
                // todo
            }
                break;
            default:
                throw "Bad LogicType";
            }
        }
    }
}
