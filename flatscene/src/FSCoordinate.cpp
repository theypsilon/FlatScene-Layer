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
	return ( _z );
}

template <typename T>
T FS3DPoint<T>::getZ() const {
	return (_z);
}

template <typename T>
void FS3DPoint<T>::setZ( T z) {
	_z= z;
}

template <typename T>
FS3DPoint<T>& FS3DPoint<T>::set ( T x , T y , T z ) {
	this->X ( ) = x ;
	this->Y ( ) = y ;
	this->Z ( ) = z ;
	return ( *this ) ;
}

template <typename T>
T FS3DPoint<T>::distance ( FS3DPoint<T>& coord ) {
	T d_x = coord.X() > this->X() ? coord.X() - this->X() : this->X() - coord.X();
	T d_y = coord.Y() > this->Y() ? coord.Y() - this->Y() : this->Y() - coord.Y();
	T d_z = coord.Z() > this->Z() ? coord.Z() - this->Z() : this->Z() - coord.Z();

	T distance = (T) sqrt((long double) (d_x*d_x + d_z*d_z + d_y*d_y));
	return distance;
}
