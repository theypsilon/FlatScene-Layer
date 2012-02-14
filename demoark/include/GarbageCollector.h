#ifndef __GARBAGE_COLLECTOR__
#define __GARBAGE_COLLECTOR__

#include "Action.h"
#include <list>
using namespace std;

class CGarbageCollector {
public:
    CGarbageCollector();
    virtual ~CGarbageCollector();
    virtual void clean()=0;
};

class CGarbageCollectorOfActions : public CGarbageCollector{
private:
    list<vector<CAction*>*> listOfBrothers;
    list<CAction*> nodes;
public:
    CGarbageCollectorOfActions();
    ~CGarbageCollectorOfActions();
    void add(vector<CAction*>* bro);
    void add(CAction* nod);
    list<CAction*>& getListActions();
    list<vector<CAction*>*>& getListBrothers();
    void clean();
};

#endif