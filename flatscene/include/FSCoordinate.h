#ifndef  __COORDINATE_H__
#define __COORDINATE_H__

#include "FSPoint.h"

template <class T>
struct FS3DPoint : public FS2DPoint<T> {
    using FS2DPoint<T>::x;
    using FS2DPoint<T>::y;

    T z;

    FS3DPoint(T x=0,T y=0,T z=0)
    : FS2DPoint<T>::FS2DPoint(x,y), z(z) {}

    FS3DPoint(FS3DPoint<T>& coord) {
        x = coord.x;
        y = coord.y;
        z = coord.z;
    }

    inline T getZ() const { return z; }
    void setZ(T nz) { z = nz; }

    FS3DPoint<T>& set(T nx,T ny,T nz) {
        x = nx;
        y = ny;
        z = nz;
        return *this;
    }

    T distance ( FS3DPoint<T>& coord ) {
        T d_x = coord.x > x ?   coord.x - x :   x - coord.x;
        T d_y = coord.y > y ?   coord.y - y :   y - coord.y;
        T d_z = coord.z > z ?   coord.z - z :   z - coord.z;

        return (d_x*d_x + d_z*d_z + d_y*d_y);
    }
};

typedef FS3DPoint<int> FSCoordinate;

#endif
