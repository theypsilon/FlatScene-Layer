#ifndef __C_COLLECTION_M__
#define __C_COLLECTION_M__

#include "FSUniverse.h"
#include "FSMessageHandler.h"
#include "FSSingleton.h"
#include "FSNoncopyable.h"
#include <map>
#include <string>
#include <memory>

namespace flatscene {

    class FSEngine;

    typedef  std::list<FSUniverse*> UniverseCollection;

    class FSMultiverse : private FSNoncopyable {
    public:
        FSUniverse* add(FSUniverse* uni,Uint8 slot=0);
        FSUniverse* universeNamed(std::string uniName,Uint8 slot=0);
        FSUniverse* universeNamed(const char* uniName,Uint8 slot=0);
        void erase(FSUniverse* uniKilled);
        void clear();
        int size();

        UniverseCollection::iterator begin();
        UniverseCollection::iterator end();

        FSMultiverse();
        ~FSMultiverse();
    private:
        friend class FSUniverse;
        struct MultiverseImpl;
        MultiverseImpl* _impl;
    };

} // flatscene

#endif
