#ifndef FS_TYPES__
#define FS_TYPES__

#include "Includes.h"

#include "Coordinate.h"
#include "Rectangle.h"

namespace FlatScene {

    typedef double Float;
    typedef unsigned char Byte;

    typedef Point2D<int> Point;
    typedef Point2D<Float> FloatPoint;

    typedef Point3D<int> Coordinate;

    typedef Rect<int> Rectangle;

} // flatscene

#endif
