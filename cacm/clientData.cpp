// clientData.cpp

#include "clientData.h"

void
clientNode::search(const char *name)
{
    stringId id(name, stringId::refString);
    pvNode *pnode = pvTable.lookup(id);
    if (!pnode) {
        pnode = new pvNode(name);
        pvTable.add(*pnode);
    }
    pnode->increment();
    latest = epicsTime::getCurrent();
    searches++;
    lifetotal++;
}

void
clientNode::report(void) const
{
    if (numSearches()) {
        printf("    %-31s\t%7.2f\t%5lu\t%5lu\n",
                identifier(), age(), numSearches(), numPVs());
    }
}

void clientNode::reportNames(void) const
{
    if (numSearches()) {
        report();
        puts("\tReq's\tPV Name");
        pvTable.traverseConst(&pvNode::report);
    }
}
