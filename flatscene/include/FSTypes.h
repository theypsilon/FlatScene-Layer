#ifndef __FS_TYPES__
#define __FS_TYPES__

#include "FSIncludes.h"

#include "FSCoordinate.h"
#include "FSRectangle.h"

namespace FlatScene {

    typedef double Float;
    typedef unsigned char Byte;

    typedef Point2D<int> Point;
    typedef Point2D<Float> FloatPoint;

    typedef Point3D<int> Coordinate;

    typedef Rect<int> Rectangle;

} // flatscene

#endif
