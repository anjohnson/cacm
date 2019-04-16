// pvData.h

#include "tsSLList.h"
#include "resourceLib.h"


class pvNode :
    public stringId,
    public tsSLNode<pvNode>
{
public:
    pvNode(const char *name) :
        stringId(name),
        count(0) {}

    ~pvNode(void) {}

    void reset(void) {
        count = 0;
    }

    void increment(void) {
        count++;
    }

    unsigned long counts(void) const {
        return count;
    }

    void report(void) const {
        if (counts() == 0) return;
        printf("\t%5lu\t%s\n", counts(), resourceName());
    }

    void reportAll(void) const {
        printf("\t%s\n", resourceName());
    }

private: // Member Data
    unsigned long count;
};


class pvNodeDestroyer :
    public tsSLList<pvNode>
{
public:
    ~pvNodeDestroyer(void) {
        while (pvNode *pnode = tsSLList<pvNode>::get()) {
            delete pnode;
        }
    }
};


