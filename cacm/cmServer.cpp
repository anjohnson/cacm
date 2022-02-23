// cmServer.cpp
// Part of CA Client Monitor by Andrew Johnson <anj@anl.gov>
// SPDX-License-Identifier: EPICS

#include <string.h>
#include <stdio.h>
#include "epicsTime.h"
#include "cmServer.h"

#define RECORD_NAME_SIZE 80
#define TIME_SIZE 32

pvExistReturn
cmServer::pvExistTest(const casCtx& ctx, const caNetAddr& addr,
    const char* pvname)
{
    clientNode *pnode = clientTable.lookup(inetAddrID(addr));
    if (!pnode) {
        epicsTime now = epicsTime::getCurrent();
        pnode = new clientNode(addr, now);
        clientTable.add(*pnode);
        
        if (show & births) {
            char when[TIME_SIZE];
            now.strftime(when, TIME_SIZE, "%Y-%m-%d %T.%09f");
            printf("%s Born=%s\n", when, pnode->identifier());
            fflush(stdout);
        }
    }
    
    char recName[RECORD_NAME_SIZE];
    strncpy(recName, pvname, RECORD_NAME_SIZE);
    char *pfield = strrchr(recName, '.');
    if (pfield) *pfield = '\0';
    
    pnode->search(recName);
    count++;
    return pverDoesNotExistHere;
}

void
cmServer::prune(const epicsTime & cutoff, unsigned long maxNames)
{
    if (numClients() == 0) return;
    
    resTable<clientNode,inetAddrID>::iterator that = clientTable.firstIter();
    resTable<clientNode,inetAddrID>::iterator next = that;
    while (that.valid()) {
        ++next;
        if (that->recent() < cutoff) {
            if (show & deaths) {
                char when[TIME_SIZE];
                that->recent().strftime(when, TIME_SIZE, "%Y-%m-%d %T.%09f");
                printf("%s Died=%s Age=%.2f Names=%lu Searches=%lu\n",
                        when, that->identifier(), that->age(), that->numPVs(),
                        that->lifeSearches());
                if (that->numPVs() <= maxNames) {
                    that->reportAll();
                }
            }
            delete clientTable.remove(*that);
        }
        that = next;
    }
    fflush(stdout);
}

void
cmServer::report(const epicsTime &now, double delay) const
{
    if (show & rates) {
        char when[TIME_SIZE];
        now.strftime(when, TIME_SIZE, "%Y-%m-%d %T.%03f");
        printf("%s Clients=%3lu Searches=%4lu Rate=%7.2f\n",
                when, numClients(), numSearches(), numSearches()/delay);
    }
    if ((show & clients) && numSearches() > 0) {
        printf("    %-31s\t%7s\t%5s\t%5s\n",
                "Client's UDP Port", "Age ", "Req's", "Names");
        if (show & names) {
            clientTable.traverseConst(&clientNode::reportNames);
        } else
            clientTable.traverseConst(&clientNode::report);
        putchar('\n');
    }
    fflush(stdout);
}
