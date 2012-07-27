#ifndef FS_IMAGES_IMPL__
#define FS_IMAGES_IMPL__

#include "Images.h"

namespace FlatScene {

struct Images::ImagesImpl {
    SpritesetCollection set;
    SpritesetCount count;

    std::stack<int> lastIndexAdded;
};

} // flatscene

#endif