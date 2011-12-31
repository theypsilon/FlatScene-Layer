#ifndef  __COORDINATE_H__
#define __COORDINATE_H__

#include "FSPoint.h"

template <class T>
class FS3DPoint : public FS2DPoint<T> {
private:
	T m_z;
public:
	FS3DPoint(T x=0,T y=0,T z=0);
	FS3DPoint(FS3DPoint<T>& coord);

	T& Z();
	inline T getZ() const ;
	void setZ(T z) ;

	FS3DPoint<T>& set(T x,T y,T z);
	T distance ( FS3DPoint<T>& coord );
};

typedef FS3DPoint<int> FSCoordinate;

#endif