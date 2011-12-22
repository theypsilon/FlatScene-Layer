//sentinel
#ifndef __POINT_H__
#define __POINT_H__

//point class
class FSPoint  
{
private:
	//x and y
	int m_x ;
	int m_y ;
public:
	//constructor
	FSPoint ( int x = 0 , int y  = 0 ) ;
	FSPoint ( FSPoint& pt ) ;

	//destructor
	virtual ~FSPoint ( ) ;

	//properties
	int& X ( ) ;
	int& Y ( ) ;

	inline int getX ( ) const ;
	inline int getY ( ) const ;
	void setX ( int x ) ;
	void setY ( int y ) ;

	//setter
	FSPoint& set ( int x , int y ) ;
	FSPoint& copy ( FSPoint& pt ) ;

	//move
	FSPoint& move ( int dx , int dy ) ;
	virtual FSPoint& add ( FSPoint& pt ) ;
	virtual FSPoint& subtract ( FSPoint& pt ) ;
	
	//scale
	virtual FSPoint& scale ( int scalar ) ;

	//distance
	virtual int distance ( FSPoint& pt ) ;

	//operators
	//assignment
	FSPoint& operator = ( FSPoint& pt ) ;
	FSPoint& operator += ( FSPoint& pt ) ;
	FSPoint& operator -= ( FSPoint& pt ) ;
	FSPoint& operator *= ( int scalar ) ;
	FSPoint& operator /= ( int scalar ) ;

	//unary
	FSPoint operator - ( ) ;
	FSPoint operator + ( ) ;

	FSPoint operator + ( FSPoint& pt ) ;
	FSPoint operator - ( FSPoint& pt ) ;
	FSPoint operator * ( int scalar ) ;
	FSPoint operator / ( int scalar ) ;

	//comparison
	bool operator == ( FSPoint& pt ) ;
	bool operator != ( FSPoint& pt ) ;

};

FSPoint operator * ( int scalar , FSPoint& pt ) ;

//sentinel
#endif
