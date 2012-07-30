#ifndef FS_REFCOUNT_MEMORY_POLICY_IMPL__
#define FS_REFCOUNT_MEMORY_POLICY_IMPL__

#include "ResourceMemoryPolicies.h"

namespace FlatScene {
    template <typename Resource, typename CountIndex>
    void RefCountMemoryPolicy<Resource, CountIndex>::destroy(Holder res) const {
        delete res;
    }
}

#endif // FS_REFCOUNT_MEMORY_POLICY_IMPL