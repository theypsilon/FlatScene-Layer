#ifndef __C_COLLECTION_M__
#define __C_COLLECTION_M__

#include "FSUniverse.h"
#include "FSMessageHandler.h"
#include "FSSingleton.h"
#include "FSNoncopyable.h"
#include <map>
#include <string>

typedef  std::list<FSUniverse*> UniverseCollection;
typedef std::map<FSMessageHandler*,UniverseCollection*> MultiverseByAdmin;

class FSMultiverse : private FSNoncopyable, public FSSingleton<FSMultiverse> {
    friend class FSSingleton<FSMultiverse>;
public:
    FSUniverse* add(FSUniverse* uni,Uint8 slot=0);
    FSUniverse* universeNamed(string uniName,Uint8 slot=0);
    FSUniverse* universeNamed(const char* uniName,Uint8 slot=0);
    void erase(FSUniverse* uniKilled);
    void clear();
    int size();

    UniverseCollection::iterator begin();
    UniverseCollection::iterator end();

private:
    FSMultiverse();
    ~FSMultiverse();

    friend class FSUniverse;
    struct MultiverseImpl;
    MultiverseImpl* _impl;
};

#ifdef GLOBAL_SINGLETON_REFERENCES
extern FSMultiverse& Cosmos;
#endif

#endif
