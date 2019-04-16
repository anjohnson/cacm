// cmServer.h

#include "casdef.h"
#include "caNetAddr.h"
#include "inetAddrID.h"	// This header is private to libca...
#include "clientData.h"

enum display {
    nothing=0,
    births=1,
    deaths=2,
    rates=4,
    clients=8,
    names=16
};

class cmServer :
    public caServer
{
public:
    cmServer(int what) :
        show(what),
        count(0) {}

    ~cmServer(void) {
        clientNodeDestroyer usedNodes;
        clientTable.removeAll(usedNodes);
    }
    
    void reset(void) {
        count = 0;
        if (numClients()) clientTable.traverse(&clientNode::reset);
    }
    
    pvExistReturn pvExistTest(const casCtx& ctx, const char* pvname) {
        return pverDoesNotExistHere;
    }

    pvExistReturn pvExistTest(const casCtx& ctx, const caNetAddr& addr,
        const char* pvname);

    void prune(const epicsTime & cutoff, unsigned long maxNames);

    unsigned long numClients(void) const {
        return clientTable.numEntriesInstalled();
    }

    unsigned long numSearches(void) const {
	return count;
    }

    void setShow(int what) {
        show = what;
    }

    void report(const epicsTime &now, double delay) const;

private: // Member Data
    resTable<clientNode,inetAddrID> clientTable;
    int show;
    unsigned long count;
};
