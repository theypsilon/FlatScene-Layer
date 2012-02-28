#ifndef __MULTIVERSE_IMPL__
#define __MULTIVERSE_IMPL__

#include "FSMultiverse.h"

struct FSMultiverse::MultiverseImpl {
    UniverseCollection unis;

    bool working;
};

#endif
