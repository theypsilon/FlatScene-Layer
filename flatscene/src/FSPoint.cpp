#include "FSPoint.h"
#include <math.h>

template <typename T>
FS2DPoint<T>::FS2DPoint (T x,T y) {
	set(x,y) ;
}

template <typename T>
FS2DPoint<T>::FS2DPoint(FS2DPoint<T>& pt) {
	setX( pt.getX() ) ;
	setY( pt.getY() ) ;
}

template <typename T>
T& FS2DPoint<T>::X() {
	return _x;
}

template <typename T>
T& FS2DPoint<T>::Y() {
	return _y;
}

template <typename T>
T FS2DPoint<T>::getX() const {
	return _x;
}

template <typename T>
T FS2DPoint<T>::getY() const {
	return _y;
}

template <typename T>
void FS2DPoint<T>::setX(T x) {
	_x = x ;
}

template <typename T>
void FS2DPoint<T>::setY(T y) {
	_y = y;
}

template <typename T>
FS2DPoint<T>& FS2DPoint<T>::set(T x , T y) {
	X() = x;
	Y() = y;
	return *this;
}

template <typename T>
T FS2DPoint<T>::distance(FS2DPoint<T>& pt) {
	T d_x = pt.X() > X() ? pt.X() - X() : X() - pt.X();
	T d_y = pt.Y() > Y() ? pt.Y() - Y() : Y() - pt.Y();

	T distance = (T) sqrt((long double) (d_x * d_x + d_y * d_y));
	return distance;
}