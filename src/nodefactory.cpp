#include "nodefactory.h"
#include "graphwidget.h"
#include <QStringList>
#include "mainwindow.h"
NodeFactory::NodeFactory()
{
}

Node *NodeFactory::Create(GraphWidget *graphWidget, LOGICTYPE lt)
{
    switch(lt) {
    case fSETUP:
         return new SetupNode(graphWidget);
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
Node *lastNode = 0;
QString Line;
    while(!s.atEnd())   {
        Line = s.readLine();
        if (Line.startsWith("//!!")) {
            QStringList TokenList = Line.split("!!");
            LOGICTYPE lt;
            if (TokenList[1]=="Group") {
                QString id = TokenList[3];
                      wp->getGroupTableItem()->AddGroupItem(id.toInt(),TokenList[2]);
                      lastNode = 0;
                      continue;
            }
            if (TokenList[1]=="fSetup") lt = fSETUP;
            if (TokenList[1]=="fIn") lt = fIN;
            if (TokenList[1]=="fOut") lt = fOUT;
            if (TokenList[1]=="fOr") lt = fOR;
            if (TokenList[1]=="fAnd") lt = fAND;
            if (TokenList[1]=="fNOT") lt = fNOT;

            if (TokenList[1]=="fEdge")  {
                lastNode = 0;
                Node *source,*dest;
                source = graphWidget->FindNode(TokenList[2]);
                dest = graphWidget->FindNode(TokenList[3]);
                if (source && dest)
                        graphWidget->scene()->addItem(new Edge(source,dest));
                continue;
            }
            if (TokenList[1]=="GID")    {
                if (lastNode!=0) {
                    for(int i=2;i<TokenList.count();i++) {
                        lastNode->setInGroup(TokenList[i].toInt(),true);
                    }
                }
                continue;
            }
            if (TokenList[1]=="INLINE") {
                if (lastNode!=0) {
                    lastNode->setINTEXT(TokenList[2]);
                }
                continue;
            }
            if (TokenList[1]=="fFuzzy") lt = fFUZZY;
            if (TokenList[1]=="fPid") lt = fPID;

            switch(lt)
            {
            case fSETUP:
            {
                SetupNode *n = new SetupNode(graphWidget);
                lastNode = n;
                n->setName(TokenList[2]);
                n->setPos(TokenList[3].toDouble(),TokenList[4].toDouble());
            }
            case fIN:
            {
                InNode *n = new InNode(graphWidget);
                lastNode = n;
                n->setName(TokenList[2]);
                n->setPos(TokenList[3].toDouble(),TokenList[4].toDouble());

            }
                break;
            case fOUT:
            {
                OutNode *n = new OutNode(graphWidget);
                lastNode = n;
                n->setName(TokenList[2]);
                n->setPos(TokenList[3].toDouble(),TokenList[4].toDouble());
            }
                break;
            case fFUZZY:
            {
             FuzzyNode *n = new FuzzyNode(graphWidget);
             lastNode = n;
                n->setName(TokenList[2]);
                for(int i=5;i<TokenList.count();i+=2)
                {
                    double cause,effect;

                    cause = TokenList[i].toDouble();
                    effect = TokenList[i+1].toDouble();
                    n->fuzzy.TFuzzyAddPoint(cause,effect);
                }
                n->setPos(TokenList[3].toDouble(),TokenList[4].toDouble());
            }

                break;
            case fAND:
            {
             AndNode *n = new AndNode(graphWidget);
             lastNode = n;
                n->setName(TokenList[2]);
                n->setActiveValue(TokenList[3].toDouble());
                n->setPos(TokenList[4].toDouble(),TokenList[5].toDouble());
            }
                break;
            case fOR:
            {
             OrNode *n = new OrNode(graphWidget);
             lastNode = n;
             n->setName(TokenList[2]);
             n->setActiveValue(TokenList[3].toDouble());
             n->setPos(TokenList[4].toDouble(),TokenList[5].toDouble());
            }
                break;
            case fNOT:
            {
                NotNode *n = new NotNode(graphWidget);
                lastNode = n;
                n->setName(TokenList[2]);
                n->setPos(TokenList[3].toDouble(),TokenList[4].toDouble());
            }
                break;
            case fPID:
            {
            PidNode *n = new PidNode(graphWidget);
            lastNode = n;
                n->setName(TokenList[2]);
                // todo
            }
                break;
            default:
                throw "Bad LogicType";
            }
        }
    }
}
