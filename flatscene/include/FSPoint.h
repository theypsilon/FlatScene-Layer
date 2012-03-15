#ifndef __POINT_H__
#define __POINT_H__

#include <math.h>

namespace flatscene {

    template <class T=int>
    struct Point2D {
        T x ;
        T y ;

        Point2D ( T x = 0 , T y  = 0 ) 
        : x(x), y(y) {}

        template <class U>
        Point2D ( const U& pt )
        : x(pt.x), y(pt.y) {}

        inline T getX() const { return x; }
        inline T getY() const { return y; }

        void setX(T nx) { x = nx; }
        void setY(T ny) { y = ny; }

        Point2D& set(T nx,T ny) {
            x = nx;
            y = ny;
            return *this;
        }

        template <class U>
        T distance(const U& pt) const {
            T d_x = pt.x > x ? pt.x - x : x - pt.x;
            T d_y = pt.y > y ? pt.y - y : y - pt.y;

            return (d_x * d_x + d_y * d_y);
        }

    };

} // flatscene

#endif
