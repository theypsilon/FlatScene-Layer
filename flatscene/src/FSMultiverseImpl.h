#ifndef __MULTIVERSE_IMPL__
#define __MULTIVERSE_IMPL__

#include "FSMultiverse.h"

using namespace flatscene;

struct FSMultiverse::MultiverseImpl {
    UniverseCollection unis;

    bool working;
};

#endif
