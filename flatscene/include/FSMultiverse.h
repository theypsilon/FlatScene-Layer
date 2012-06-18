#ifndef __C_COLLECTION_M__
#define __C_COLLECTION_M__

#include "FSUniverse.h"
#include "FSMessageHandler.h"
#include "FSSingleton.h"
#include "FSNoncopyable.h"
#include <map>
#include <string>
#include <memory>

namespace FlatScene {

    class Engine;

    typedef  std::list<Universe*> UniverseCollection;

    class Multiverse : private Noncopyable {
    public:
        Universe* add(Universe* uni,Uint8 slot=0);
        Universe* universeNamed(std::string uniName,Uint8 slot=0);
        Universe* universeNamed(const char* uniName,Uint8 slot=0);
        void erase(Universe* uniKilled);
        void clear();
        int size();

        UniverseCollection::iterator begin();
        UniverseCollection::iterator end();

        Multiverse();
        ~Multiverse();
    private:
        friend class Universe;
        struct MultiverseImpl;
        MultiverseImpl* _impl;
    };

} // flatscene

#endif
