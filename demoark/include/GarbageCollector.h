#ifndef __GARBAGE_COLLECTOR__
#define __GARBAGE_COLLECTOR__

#include "Action.h"
#include <list>

class CGarbageCollector {
public:
    CGarbageCollector();
    virtual ~CGarbageCollector();
    virtual void clean()=0;
};

class CGarbageCollectorOfActions : public CGarbageCollector{
private:
    std::list<std::vector<CAction*>*> listOfBrothers;
    std::list<CAction*> nodes;
public:
    CGarbageCollectorOfActions();
    ~CGarbageCollectorOfActions();
    void add(std::vector<CAction*>* bro);
    void add(CAction* nod);
    std::list<CAction*>& getListActions();
    std::list<std::vector<CAction*>*>& getListBrothers();
    void clean();
};

#endif