#ifndef __POINT_H__
#define __POINT_H__

template <class T=int>
class FS2DPoint {
protected:
	T _x ;
	T _y ;
public:
	FS2DPoint ( T x = 0 , T y  = 0 );
	FS2DPoint ( FS2DPoint<T>& pt );

	T& X();
	T& Y();

	T getX() const;
	T getY() const;
	void setX(T x);
	void setY(T y);

	FS2DPoint& set(T x,T y);

	virtual T distance(FS2DPoint& pt);
};

typedef FS2DPoint<int> FSPoint;
typedef FS2DPoint<float> FSFloatPoint;

#endif
