#ifndef __POINT_H__
#define __POINT_H__

#include <math.h>

template <class T=int>
struct FS2DPoint {
    T x ;
    T y ;

    FS2DPoint ( T x = 0 , T y  = 0 ) 
    : x(x), y(y) {}

    template <class U>
    FS2DPoint ( const U& pt )
    : x(pt.x), y(pt.y) {}

    inline T getX() const { return x; }
    inline T getY() const { return y; }

    void setX(T nx) { x = nx; }
    void setY(T ny) { y = ny; }

    FS2DPoint& set(T nx,T ny) {
        x = nx;
        y = ny;
        return *this;
    }

    template <class U>
    T distance(const U& pt) {
        T d_x = pt.x > x ? pt.x - x : x - pt.x;
        T d_y = pt.y > y ? pt.y - y : y - pt.y;

        return (d_x * d_x + d_y * d_y);
    }

};

typedef FS2DPoint<int> FSPoint;
typedef FS2DPoint<float> FSFloatPoint;

#endif
