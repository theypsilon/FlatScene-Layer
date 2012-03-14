#ifndef __FS_TYPES__
#define __FS_TYPES__

#include "FSCoordinate.h"
#include "FSRectangle.h"

namespace flatscene {

    typedef double Float;

    typedef FS2DPoint<int> FSPoint;
    typedef FS2DPoint<Float> FSFloatPoint;

    typedef FS3DPoint<int> FSCoordinate;

    typedef FSRect<int> FSRectangle;

} // flatscene

#endif