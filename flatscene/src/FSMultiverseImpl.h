#ifndef __MULTIVERSE_IMPL__
#define __MULTIVERSE_IMPL__

#include "FSMultiverse.h"

struct FSMultiverse::MultiverseImpl {
    std::shared_ptr<FSEngine> admin;
    MultiverseByAdmin session;
    UniverseCollection* unisCurrent;

    std::shared_ptr<FSEngine> setAdmin(std::shared_ptr<FSEngine> newAdmin);

    bool working;
};

#endif
