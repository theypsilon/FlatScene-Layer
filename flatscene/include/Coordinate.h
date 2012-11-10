#ifndef  __COORDINATE_H__
#define __COORDINATE_H__

#include "Point.h"

namespace FlatScene {

    template <class T>
    struct Point3D : public Point2D<T> {
        using Point2D<T>::x;
        using Point2D<T>::y;

        T z;

        Point3D(T nx=0,T ny=0,T nz=0)
        : Point2D<T>::Point2D{nx,ny}, z{nz} {}

        template <class U>
        Point3D(const U& coord) Point3D{coord.x,coord.y,coord.z} {}

        inline T getZ() const { return z; }
        void setZ(T nz) { z = nz; }

        Point3D<T>& set(T nx,T ny,T nz) {
            x = nx;
            y = ny;
            z = nz;
            return *this;
        }
    
        template <class U>
        T distance ( const U& coord ) const {
            T d_x = coord.x > x ?   coord.x - x :   x - coord.x;
            T d_y = coord.y > y ?   coord.y - y :   y - coord.y;
            T d_z = coord.z > z ?   coord.z - z :   z - coord.z;

            return (d_x*d_x + d_z*d_z + d_y*d_y);
        }
    };

} // flatscene

#endif
