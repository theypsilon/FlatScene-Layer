#ifndef __FS_IMAGES_IMPL__
#define __FS_IMAGES_IMPL__

#include "FSImages.h"

struct FSImages::ImagesImpl {
    SpritesetCollection set;
    SpritesetCount count;

    std::stack<int> lastIndexAdded;
};

#endif
