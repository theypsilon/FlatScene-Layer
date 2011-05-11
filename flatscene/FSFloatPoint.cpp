#include "FSFloatPoint.h"
#include <math.h>

//constructor
CFloatPoint::CFloatPoint ( float x , float y ) 
{
	//set the x and y of the Point
	set ( x , y ) ;
}

CFloatPoint::CFloatPoint ( CFloatPoint& pt ) 
{
	//set x and y of the Point
	setX ( pt.getX ( ) ) ;
	setY ( pt.getY ( ) ) ;
}


//destructor
CFloatPoint::~CFloatPoint ( ) 
{
	//do nothing
}

//properties
float& CFloatPoint::X ( ) 
{
	//return x componEnt
	return ( m_x ) ;
}

float& CFloatPoint::Y ( ) 
{
	//return y componEnt
	return ( m_y ) ;
}

float CFloatPoint::getX ( ) const
{
	//return x componEnt
	return ( m_x ) ;
}

float CFloatPoint::getY ( ) const
{
	//return y componEnt
	return ( m_y ) ;
}

void CFloatPoint::setX ( float x ) 
{
	//assign new value to x componEnt
	m_x = x ;
}

void CFloatPoint::setY ( float y ) 
{
	//assign new value to y componEnt
	m_y = y ;
}

//setter
CFloatPoint& CFloatPoint::set ( float x , float y ) 
{
	//set x and y
	X ( ) = x ;
	Y ( ) = y ;
	//return this Point
	return ( *this ) ;
}

CFloatPoint& CFloatPoint::copy ( CFloatPoint& pt ) 
{
	//copy x and y
	X ( ) = pt.X ( ) ;
	Y ( ) = pt.Y ( ) ;
	//return this Point
	return ( *this ) ;
}

//move
CFloatPoint& CFloatPoint::move ( float dx , float dy ) 
{
	//move x and y
	X ( ) += dx ;
	Y ( ) += dy ;
	//return this Point
	return ( *this ) ;
}

CFloatPoint& CFloatPoint::add ( CFloatPoint& pt ) 
{
	//add Point to this Point
	( *this ) += pt ;
	//return this Point
	return ( *this ) ;
}

CFloatPoint& CFloatPoint::subtract ( CFloatPoint& pt ) 
{
	//subtract Point from this Point
	( *this ) -= pt ;
	//return this Point
	return ( *this ) ;
}

//scale
CFloatPoint& CFloatPoint::scale ( float scalar ) 
{
	//multiply this Point by a scalar
	( *this ) *= scalar ;
	//return this Point
	return ( *this ) ;
}

//distance
float CFloatPoint::distance ( CFloatPoint& pt ) 
{
	//calculate distance
	float distance = X ( ) * X ( ) + Y ( ) * Y ( ) ;
	distance = ( float ) sqrt ( (float) distance ) ;
	//return the distance
	return ( distance ) ;
}

//operators
//assignment
CFloatPoint& CFloatPoint::operator = ( CFloatPoint& pt ) 
{
	//copy x and y
	X ( ) = pt.X ( ) ;
	Y ( ) = pt.Y ( ) ;
	//return this Point
	return ( *this ) ;
}

CFloatPoint& CFloatPoint::operator += ( CFloatPoint& pt ) 
{
	//add to x and y
	X ( ) += pt.X ( ) ;
	Y ( ) += pt.Y ( ) ;
	//return this Point
	return ( *this ) ;
}

CFloatPoint& CFloatPoint::operator -= ( CFloatPoint& pt ) 
{
	//subtract x and y
	X ( ) -= pt.X ( ) ;
	Y ( ) -= pt.Y ( ) ;
	//return this Point
	return ( *this ) ;
}

CFloatPoint& CFloatPoint::operator *= ( float scalar ) 
{
	//multiply by scalar
	X ( ) *= scalar ;
	Y ( ) *= scalar ;
	//return this Point
	return ( *this ) ;
}

CFloatPoint& CFloatPoint::operator /= ( float scalar ) 
{
	//divide by scalar
	X ( ) /= scalar ;
	//return this Point
	return ( *this ) ;
}

//unary
CFloatPoint CFloatPoint::operator - ( ) 
{
	//unary negativeness--multiply by -1
	CFloatPoint pt = ( *this ) ;
	pt *= -1 ;
	return ( pt ) ;
}

CFloatPoint CFloatPoint::operator + ( ) 
{
	//unary positiveness
	return ( *this ) ;
}

CFloatPoint CFloatPoint::operator + ( CFloatPoint& pt ) 
{
	//add Points togetHer
	CFloatPoint result ;
	result = ( *this ) ;
	result += pt ;
	//return new Point
	return ( result ) ;
}

CFloatPoint CFloatPoint::operator - ( CFloatPoint& pt ) 
{
	//subtract Points
	CFloatPoint result ;
	result = ( *this ) ;
	result -= pt ;
	//return new Point
	return ( result ) ;
}

CFloatPoint CFloatPoint::operator * ( float scalar ) 
{
	//multiply Point by scalar
	CFloatPoint result ;
	result = ( *this ) ;
	result *= scalar ;
	//return new Point
	return ( result ) ;
}

CFloatPoint CFloatPoint::operator / ( float scalar ) 
{
	//divide by scalar
	CFloatPoint result ;
	result = ( *this ) ;
	result /= scalar ;
	//return new Point
	return ( result ) ;
}

//comparison
bool CFloatPoint::operator == ( CFloatPoint& pt ) 
{
	//check for equality
	return ( X ( ) == pt.X ( ) && Y ( ) == pt.Y ( ) ) ;
}

bool CFloatPoint::operator != ( CFloatPoint& pt ) 
{
	//check for inequality
	return ( !( ( *this ) == pt ) ) ;
}

CFloatPoint operator * ( float scalar , CFloatPoint& pt ) 
{
	//scalar*pt rather than pt*scalar, still the same idea...
	CFloatPoint result (pt);
	pt *= scalar ;
	return ( result ) ;
}
