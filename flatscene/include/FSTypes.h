#ifndef __FS_TYPES__
#define __FS_TYPES__

#include "FSCoordinate.h"
#include "FSRectangle.h"

#include "SDL.h"

namespace flatscene {

    typedef double Float;
    typedef unsigned char Byte;

    typedef Point2D<int> Point;
    typedef Point2D<Float> FloatPoint;

    typedef Point3D<int> Coordinate;

    typedef Rect<int> Rectangle;

} // flatscene

#endif
