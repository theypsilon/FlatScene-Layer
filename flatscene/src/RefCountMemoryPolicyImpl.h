#ifndef FS_REFCOUNT_MEMORY_POLICY_IMPL__
#define FS_REFCOUNT_MEMORY_POLICY_IMPL__

#include "ResourceMemoryPolicies.h"

namespace FlatScene {
    template <typename Resource, typename HolderPolicy, typename CountIndex>
    void RefCountMemoryPolicy<Resource,HolderPolicy, CountIndex>::destroy(Holder res) {
        delete res;
    }
}

#endif // FS_REFCOUNT_MEMORY_POLICY_IMPL