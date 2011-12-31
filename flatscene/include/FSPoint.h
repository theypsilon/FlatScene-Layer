#ifndef __POINT_H__
#define __POINT_H__

template <typename T=int>
class FS2DPoint {
private:
	T m_x ;
	T m_y ;
public:
	FS2DPoint ( T x = 0 , T y  = 0 );
	FS2DPoint ( FS2DPoint& pt );

	T& X();
	T& Y();

	inline T getX() const;
	inline T getY() const;
	void setX(T x);
	void setY(T y);

	FS2DPoint& set(T x,T y);

	virtual T distance(FS2DPoint& pt);
};

typedef FS2DPoint<int> FSPoint;

#endif
