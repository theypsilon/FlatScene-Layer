#ifndef __MULTIVERSE_IMPL__
#define __MULTIVERSE_IMPL__

#include "FSMultiverse.h"

struct FSMultiverse::MultiverseImpl {
    FSMessageHandler* admin;
    MultiverseByAdmin session;
    UniverseCollection* unisCurrent;

    FSMessageHandler* setAdmin(FSMessageHandler* newAdmin);

    bool working;
};

#endif
