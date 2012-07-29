#ifndef FS_REFCOUNT_MEMORY_POLICY_IMPL__
#define FS_REFCOUNT_MEMORY_POLICY_IMPL__

#include "RefCountMemoryPolicy.h"

namespace FlatScene {
    template <typename Resource, typename CountIndex>
    void RefCountMemoryPolicy<Resource,CountIndex>::destroy(Holder res) {
        assert(_count.find(res) != _count.end());
        _count.erase(res);
        delete res;
    }
}

#endif // FS_REFCOUNT_MEMORY_POLICY_IMPL