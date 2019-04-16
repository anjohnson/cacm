// clientData.h

#include "caNetAddr.h"
#include "inetAddrID.h" // This header is private to libca...
#include "tsSLList.h"
#include "resourceLib.h"
#include "epicsTime.h"
#include "pvData.h"

#define IPADDR_SIZE 32

class clientNode :
    public inetAddrID,
    public tsSLNode<clientNode>
{
public:
    clientNode(const caNetAddr& clientAddr, epicsTime now) :
        inetAddrID(clientAddr),
        born(now),
        searches(0),
        lifetotal(0) {
        clientAddr.stringConvert(ipAddr, IPADDR_SIZE);
    }

    ~clientNode(void) {
        pvNodeDestroyer usedNodes;
        pvTable.removeAll(usedNodes);
    }

    void reset(void) {
        pvTable.traverse(&pvNode::reset);
        searches = 0;
    }

    void search(const char *name);

    unsigned long numPVs(void) const {
        return pvTable.numEntriesInstalled();
    }

    unsigned long numSearches(void) const {
        return searches;
    }

    unsigned long lifeSearches(void) const {
        return lifetotal;
    }

    const epicsTime & recent(void) const {
        return latest;
    }

    double age(void) const {
        return latest - born;
    }

    const char *identifier(void) const {
        return ipAddr;
    }

    void report(void) const;
    void reportNames(void) const;
    
    void reportAll(void) const {
        pvTable.traverseConst(&pvNode::reportAll);
    };

private: // Member Data
    char ipAddr[IPADDR_SIZE];
    const epicsTime born;
    epicsTime latest;
    unsigned long searches;
    unsigned long lifetotal;
    resTable<pvNode,stringId> pvTable;
};


class clientNodeDestroyer :
    public tsSLList<clientNode>
{
public:
    ~clientNodeDestroyer(void) {
        while (clientNode *pnode = tsSLList<clientNode>::get()) {
            delete pnode;
        }
    }
};
