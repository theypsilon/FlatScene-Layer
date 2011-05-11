//sentinel
#ifndef __POINT_H__
#define __POINT_H__

//point class
class CPoint  
{
private:
	//x and y
	int m_x ;
	int m_y ;
public:
	//constructor
	CPoint ( int x = 0 , int y  = 0 ) ;
	CPoint ( CPoint& pt ) ;

	//destructor
	virtual ~CPoint ( ) ;

	//properties
	int& X ( ) ;
	int& Y ( ) ;

	inline int getX ( ) const ;
	inline int getY ( ) const ;
	void setX ( int x ) ;
	void setY ( int y ) ;

	//setter
	CPoint& set ( int x , int y ) ;
	CPoint& copy ( CPoint& pt ) ;

	//move
	CPoint& move ( int dx , int dy ) ;
	virtual CPoint& add ( CPoint& pt ) ;
	virtual CPoint& subtract ( CPoint& pt ) ;
	
	//scale
	virtual CPoint& scale ( int scalar ) ;

	//distance
	virtual int distance ( CPoint& pt ) ;

	//operators
	//assignment
	CPoint& operator = ( CPoint& pt ) ;
	CPoint& operator += ( CPoint& pt ) ;
	CPoint& operator -= ( CPoint& pt ) ;
	CPoint& operator *= ( int scalar ) ;
	CPoint& operator /= ( int scalar ) ;

	//unary
	CPoint operator - ( ) ;
	CPoint operator + ( ) ;

	CPoint operator + ( CPoint& pt ) ;
	CPoint operator - ( CPoint& pt ) ;
	CPoint operator * ( int scalar ) ;
	CPoint operator / ( int scalar ) ;

	//comparison
	bool operator == ( CPoint& pt ) ;
	bool operator != ( CPoint& pt ) ;

};

CPoint operator * ( int scalar , CPoint& pt ) ;

//sentinel
#endif
