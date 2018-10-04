#ifndef NODEFACTORY_H
#define NODEFACTORY_H


#include "node.h"
#include "innode.h"
#include "outnode.h"
#include "fuzzynode.h"
#include "andnode.h"
#include "ornode.h"
#include "pidnode.h"
#include "mapnode.h"


class NodeFactory
{
public:
    NodeFactory();
    static Node *Create(GraphWidget *graphWidget,LOGICTYPE lt);

    static void ReadSource(GraphWidget *graphWidget, QTextStream &s);
};

#endif // NODEFACTORY_H
