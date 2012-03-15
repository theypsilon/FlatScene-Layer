#ifndef __FS_IMAGES_IMPL__
#define __FS_IMAGES_IMPL__

#include "FSImages.h"

namespace flatscene {

struct Images::ImagesImpl {
    SpritesetCollection set;
    SpritesetCount count;

    std::stack<int> lastIndexAdded;
};

} // flatscene

#endif
