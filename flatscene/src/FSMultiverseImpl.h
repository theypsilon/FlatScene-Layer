#ifndef __MULTIVERSE_IMPL__
#define __MULTIVERSE_IMPL__

#include "FSMultiverse.h"

namespace flatscene {

struct Multiverse::MultiverseImpl {
    UniverseCollection unis;

    bool working;
};

} // flatscene

#endif
