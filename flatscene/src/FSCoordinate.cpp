#include "FSCoordinate.h"
#include <math.h>

template <typename T>
FS3DPoint<T>::FS3DPoint ( T x , T y , T z ) : FSPoint(x,y) {
	setZ( z );
}

template <typename T>
FS3DPoint<T>::FS3DPoint ( FS3DPoint<T>& coord ) {
	setX ( coord.getX ( ) ) ;
	setY ( coord.getY ( ) ) ;
	setZ ( coord.getZ ( ) ) ;
}

template <typename T>
T& FS3DPoint<T>::Z ( ) {
	return ( m_z );
}

template <typename T>
T FS3DPoint<T>::getZ() const {
	return (m_z);
}

template <typename T>
void FS3DPoint<T>::setZ( T z) {
	m_z= z;
}

template <typename T>
FS3DPoint<T>& FS3DPoint<T>::set ( T x , T y , T z ) {
	X ( ) = x ;
	Y ( ) = y ;
	Z ( ) = z ;
	return ( *this ) ;
}

template <typename T>
T FS3DPoint<T>::distance ( FS3DPoint<T>& coord ) {
	T d_x = pt.X() > X() ? pt.X() - X() : X() - pt.X();
	T d_y = pt.Y() > Y() ? pt.Y() - Y() : Y() - pt.Y();
	T d_z = pt.Z() > Z() ? pt.Z() - Z() : Z() - pt.Z();

	T distance = (T) sqrt((long double) (d_x*d_x + d_z*d_z + d_y*d_y))
	return distance;
}