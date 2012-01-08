#ifndef __POINT_H__
#define __POINT_H__

#include <math.h>

template <class T=int>
struct FS2DPoint {
	T x ;
	T y ;
	
    FS2DPoint ( T x = 0 , T y  = 0 ) 
    : x(x), y(y) {}

	FS2DPoint ( FS2DPoint<T>& pt ) {
        x = pt.x;
        y = pt.y;
    }

	inline T getX() const {
        return x;
    }

	inline T getY() const {
        return y;
    }

    inline T& X() {
        return x;
    }

    inline T& Y() {
        return y;
    }

	void setX(T x) {
        this->x = x;
    }

	void setY(T y) {
        this->y = y;
    }

	FS2DPoint& set(T x,T y) {
        this->x = x;
        this->y = y;
		return *this;
    }

	virtual T distance(FS2DPoint& pt) {
        T d_x = pt.x > x ? pt.x - x : x - pt.x;
        T d_y = pt.y > y ? pt.y - y : y - pt.y;

        return (d_x * d_x + d_y * d_y);
    }

};

typedef FS2DPoint<int> FSPoint;
typedef FS2DPoint<float> FSFloatPoint;

#endif
