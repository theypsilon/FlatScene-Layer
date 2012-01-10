#ifndef __MULTIVERSE_IMPL__
#define __MULTIVERSE_IMPL__

#include "FSControlMultiverse.h"

struct FSControlMultiverse::MultiverseImpl {
    FSMessageHandler* admin;
    MultiverseByAdmin session;
    UniverseCollection* unisCurrent;

    FSMessageHandler* setAdmin(FSMessageHandler* newAdmin);

    static FSControlMultiverse* singleton;

    bool working;
};

#endif
