//sentinel
#ifndef __FLOAT_Point_H__
#define __FLOAT_Point_H__

//Point class
class FSFloatPoint  
{
private:
	//x and y
	float m_x ;
	float m_y ;
public:
	//constructor
	FSFloatPoint ( float x = 0 , float y  = 0 ) ;
	FSFloatPoint ( FSFloatPoint& pt ) ;

	//destructor
	virtual ~FSFloatPoint ( ) ;

	//properties
	float& X ( ) ;
	float& Y ( ) ;

	inline float getX ( ) const ;
	inline float getY ( ) const ;
	void setX ( float x ) ;
	void setY ( float y ) ;

	//setter
	FSFloatPoint& set ( float x , float y ) ;
	FSFloatPoint& copy ( FSFloatPoint& pt ) ;

	//move
	FSFloatPoint& move ( float dx , float dy ) ;
	virtual FSFloatPoint& add ( FSFloatPoint& pt ) ;
	virtual FSFloatPoint& subtract ( FSFloatPoint& pt ) ;
	
	//scale
	virtual FSFloatPoint& scale ( float scalar ) ;

	//distance
	virtual float distance ( FSFloatPoint& pt ) ;

	//operators
	//assignment
	FSFloatPoint& operator = ( FSFloatPoint& pt ) ;
	FSFloatPoint& operator += ( FSFloatPoint& pt ) ;
	FSFloatPoint& operator -= ( FSFloatPoint& pt ) ;
	FSFloatPoint& operator *= ( float scalar ) ;
	FSFloatPoint& operator /= ( float scalar ) ;

	//unary
	FSFloatPoint operator - ( ) ;
	FSFloatPoint operator + ( ) ;

	FSFloatPoint operator + ( FSFloatPoint& pt ) ;
	FSFloatPoint operator - ( FSFloatPoint& pt ) ;
	FSFloatPoint operator * ( float scalar ) ;
	FSFloatPoint operator / ( float scalar ) ;

	//comparison
	bool operator == ( FSFloatPoint& pt ) ;
	bool operator != ( FSFloatPoint& pt ) ;

};

FSFloatPoint operator * ( float scalar , FSFloatPoint& pt ) ;

//sentinel
#endif
