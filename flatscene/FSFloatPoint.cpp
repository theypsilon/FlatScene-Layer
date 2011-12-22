#include "FSFloatPoint.h"
#include <math.h>

//constructor
FSFloatPoint::FSFloatPoint ( float x , float y ) 
{
	//set the x and y of the Point
	set ( x , y ) ;
}

FSFloatPoint::FSFloatPoint ( FSFloatPoint& pt ) 
{
	//set x and y of the Point
	setX ( pt.getX ( ) ) ;
	setY ( pt.getY ( ) ) ;
}


//destructor
FSFloatPoint::~FSFloatPoint ( ) 
{
	//do nothing
}

//properties
float& FSFloatPoint::X ( ) 
{
	//return x componEnt
	return ( m_x ) ;
}

float& FSFloatPoint::Y ( ) 
{
	//return y componEnt
	return ( m_y ) ;
}

float FSFloatPoint::getX ( ) const
{
	//return x componEnt
	return ( m_x ) ;
}

float FSFloatPoint::getY ( ) const
{
	//return y componEnt
	return ( m_y ) ;
}

void FSFloatPoint::setX ( float x ) 
{
	//assign new value to x componEnt
	m_x = x ;
}

void FSFloatPoint::setY ( float y ) 
{
	//assign new value to y componEnt
	m_y = y ;
}

//setter
FSFloatPoint& FSFloatPoint::set ( float x , float y ) 
{
	//set x and y
	X ( ) = x ;
	Y ( ) = y ;
	//return this Point
	return ( *this ) ;
}

FSFloatPoint& FSFloatPoint::copy ( FSFloatPoint& pt ) 
{
	//copy x and y
	X ( ) = pt.X ( ) ;
	Y ( ) = pt.Y ( ) ;
	//return this Point
	return ( *this ) ;
}

//move
FSFloatPoint& FSFloatPoint::move ( float dx , float dy ) 
{
	//move x and y
	X ( ) += dx ;
	Y ( ) += dy ;
	//return this Point
	return ( *this ) ;
}

FSFloatPoint& FSFloatPoint::add ( FSFloatPoint& pt ) 
{
	//add Point to this Point
	( *this ) += pt ;
	//return this Point
	return ( *this ) ;
}

FSFloatPoint& FSFloatPoint::subtract ( FSFloatPoint& pt ) 
{
	//subtract Point from this Point
	( *this ) -= pt ;
	//return this Point
	return ( *this ) ;
}

//scale
FSFloatPoint& FSFloatPoint::scale ( float scalar ) 
{
	//multiply this Point by a scalar
	( *this ) *= scalar ;
	//return this Point
	return ( *this ) ;
}

//distance
float FSFloatPoint::distance ( FSFloatPoint& pt ) 
{
	//calculate distance
	float distance = X ( ) * X ( ) + Y ( ) * Y ( ) ;
	distance = ( float ) sqrt ( (float) distance ) ;
	//return the distance
	return ( distance ) ;
}

//operators
//assignment
FSFloatPoint& FSFloatPoint::operator = ( FSFloatPoint& pt ) 
{
	//copy x and y
	X ( ) = pt.X ( ) ;
	Y ( ) = pt.Y ( ) ;
	//return this Point
	return ( *this ) ;
}

FSFloatPoint& FSFloatPoint::operator += ( FSFloatPoint& pt ) 
{
	//add to x and y
	X ( ) += pt.X ( ) ;
	Y ( ) += pt.Y ( ) ;
	//return this Point
	return ( *this ) ;
}

FSFloatPoint& FSFloatPoint::operator -= ( FSFloatPoint& pt ) 
{
	//subtract x and y
	X ( ) -= pt.X ( ) ;
	Y ( ) -= pt.Y ( ) ;
	//return this Point
	return ( *this ) ;
}

FSFloatPoint& FSFloatPoint::operator *= ( float scalar ) 
{
	//multiply by scalar
	X ( ) *= scalar ;
	Y ( ) *= scalar ;
	//return this Point
	return ( *this ) ;
}

FSFloatPoint& FSFloatPoint::operator /= ( float scalar ) 
{
	//divide by scalar
	X ( ) /= scalar ;
	//return this Point
	return ( *this ) ;
}

//unary
FSFloatPoint FSFloatPoint::operator - ( ) 
{
	//unary negativeness--multiply by -1
	FSFloatPoint pt = ( *this ) ;
	pt *= -1 ;
	return ( pt ) ;
}

FSFloatPoint FSFloatPoint::operator + ( ) 
{
	//unary positiveness
	return ( *this ) ;
}

FSFloatPoint FSFloatPoint::operator + ( FSFloatPoint& pt ) 
{
	//add Points togetHer
	FSFloatPoint result ;
	result = ( *this ) ;
	result += pt ;
	//return new Point
	return ( result ) ;
}

FSFloatPoint FSFloatPoint::operator - ( FSFloatPoint& pt ) 
{
	//subtract Points
	FSFloatPoint result ;
	result = ( *this ) ;
	result -= pt ;
	//return new Point
	return ( result ) ;
}

FSFloatPoint FSFloatPoint::operator * ( float scalar ) 
{
	//multiply Point by scalar
	FSFloatPoint result ;
	result = ( *this ) ;
	result *= scalar ;
	//return new Point
	return ( result ) ;
}

FSFloatPoint FSFloatPoint::operator / ( float scalar ) 
{
	//divide by scalar
	FSFloatPoint result ;
	result = ( *this ) ;
	result /= scalar ;
	//return new Point
	return ( result ) ;
}

//comparison
bool FSFloatPoint::operator == ( FSFloatPoint& pt ) 
{
	//check for equality
	return ( X ( ) == pt.X ( ) && Y ( ) == pt.Y ( ) ) ;
}

bool FSFloatPoint::operator != ( FSFloatPoint& pt ) 
{
	//check for inequality
	return ( !( ( *this ) == pt ) ) ;
}

FSFloatPoint operator * ( float scalar , FSFloatPoint& pt ) 
{
	//scalar*pt rather than pt*scalar, still the same idea...
	FSFloatPoint result (pt);
	pt *= scalar ;
	return ( result ) ;
}
