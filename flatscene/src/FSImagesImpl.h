#ifndef __FS_IMAGES_IMPL__
#define __FS_IMAGES_IMPL__

#include "FSControlImages.h"

struct FSControlImages::ImagesImpl {
    SpritesetCollection set;
    SpritesetCount count;

    stack<int> lastIndexAdded;

    static FSControlImages* singleton;
};

#endif
