//include guards
#ifndef FS_LIBRARY_H__
#define FS_LIBRARY_H__

#include "Screen.h"

namespace FlatScene {

    namespace Library {

        inline int init( int width , int height , int bpp , bool fullscreen, bool doublebuff ) {
            return Screen::I().start(width,height,bpp,fullscreen,doublebuff);
        }

    }

} // flatscene

#endif //#ifndef FS_LIBRARY_H__
