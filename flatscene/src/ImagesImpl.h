#ifndef FS_IMAGES_IMPL__
#define FS_IMAGES_IMPL__

#include "Images.h"

namespace FlatScene {

struct Images::ImagesImpl {
    SpritesetCollection set;
    SpritesetCount count;

    std::stack<int> lastIndexAdded;
};

    template <typename Resource, typename CountIndex>
    void RefCountMemoryPolicy<Resource,CountIndex>::destroy(Holder res) {
        assert(_count.find(res) != _count.end());
        _count.erase(res);
        delete res;
    }

} // flatscene

#endif
