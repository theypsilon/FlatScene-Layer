#ifndef FS_IMAGE_ADAPTER__
#define FS_IMAGE_ADAPTER__

#include "Includes.h"

namespace FlatScene {

    typedef SDL_Surface*        RawImageResource;
    typedef const SDL_Surface&  ConstRawImageResource;

    typedef SDL_Rect            RectangleImage;


} // FlatScene;

#endif // FS_IMAGE_ADAPTER__