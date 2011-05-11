//sentinel
#ifndef __FLOAT_Point_H__
#define __FLOAT_Point_H__

//Point class
class CFloatPoint  
{
private:
	//x and y
	float m_x ;
	float m_y ;
public:
	//constructor
	CFloatPoint ( float x = 0 , float y  = 0 ) ;
	CFloatPoint ( CFloatPoint& pt ) ;

	//destructor
	virtual ~CFloatPoint ( ) ;

	//properties
	float& X ( ) ;
	float& Y ( ) ;

	inline float getX ( ) const ;
	inline float getY ( ) const ;
	void setX ( float x ) ;
	void setY ( float y ) ;

	//setter
	CFloatPoint& set ( float x , float y ) ;
	CFloatPoint& copy ( CFloatPoint& pt ) ;

	//move
	CFloatPoint& move ( float dx , float dy ) ;
	virtual CFloatPoint& add ( CFloatPoint& pt ) ;
	virtual CFloatPoint& subtract ( CFloatPoint& pt ) ;
	
	//scale
	virtual CFloatPoint& scale ( float scalar ) ;

	//distance
	virtual float distance ( CFloatPoint& pt ) ;

	//operators
	//assignment
	CFloatPoint& operator = ( CFloatPoint& pt ) ;
	CFloatPoint& operator += ( CFloatPoint& pt ) ;
	CFloatPoint& operator -= ( CFloatPoint& pt ) ;
	CFloatPoint& operator *= ( float scalar ) ;
	CFloatPoint& operator /= ( float scalar ) ;

	//unary
	CFloatPoint operator - ( ) ;
	CFloatPoint operator + ( ) ;

	CFloatPoint operator + ( CFloatPoint& pt ) ;
	CFloatPoint operator - ( CFloatPoint& pt ) ;
	CFloatPoint operator * ( float scalar ) ;
	CFloatPoint operator / ( float scalar ) ;

	//comparison
	bool operator == ( CFloatPoint& pt ) ;
	bool operator != ( CFloatPoint& pt ) ;

};

CFloatPoint operator * ( float scalar , CFloatPoint& pt ) ;

//sentinel
#endif
