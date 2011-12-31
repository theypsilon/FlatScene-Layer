#include "FSPoint.h"
#include <math.h>

//constructor
FSPoint::FSPoint ( int x , int y ) 
{
	//set the x and y of the point
	set ( x , y ) ;
}

FSPoint::FSPoint ( FSPoint& pt ) 
{
	//set x and y of the point
	setX ( pt.getX ( ) ) ;
	setY ( pt.getY ( ) ) ;
}


//destructor
FSPoint::~FSPoint ( ) 
{
	//do nothing
}

//properties
int& FSPoint::X ( ) 
{
	//return x componEnt
	return ( m_x ) ;
}

int& FSPoint::Y ( ) 
{
	//return y componEnt
	return ( m_y ) ;
}

int FSPoint::getX ( ) const
{
	//return x componEnt
	return ( m_x ) ;
}

int FSPoint::getY ( ) const
{
	//return y componEnt
	return ( m_y ) ;
}

void FSPoint::setX ( int x ) 
{
	//assign new value to x componEnt
	m_x = x ;
}

void FSPoint::setY ( int y ) 
{
	//assign new value to y componEnt
	m_y = y ;
}

//setter
FSPoint& FSPoint::set ( int x , int y ) 
{
	//set x and y
	X ( ) = x ;
	Y ( ) = y ;
	//return this point
	return ( *this ) ;
}

FSPoint& FSPoint::copy ( FSPoint& pt ) 
{
	//copy x and y
	X ( ) = pt.X ( ) ;
	Y ( ) = pt.Y ( ) ;
	//return this point
	return ( *this ) ;
}

//move
FSPoint& FSPoint::move ( int dx , int dy ) 
{
	//move x and y
	X ( ) += dx ;
	Y ( ) += dy ;
	//return this point
	return ( *this ) ;
}

FSPoint& FSPoint::add ( FSPoint& pt ) 
{
	//add point to this point
	( *this ) += pt ;
	//return this point
	return ( *this ) ;
}

FSPoint& FSPoint::subtract ( FSPoint& pt ) 
{
	//subtract point from this point
	( *this ) -= pt ;
	//return this point
	return ( *this ) ;
}

//scale
FSPoint& FSPoint::scale ( int scalar ) 
{
	//multiply this point by a scalar
	( *this ) *= scalar ;
	//return this point
	return ( *this ) ;
}

//distance
int FSPoint::distance ( FSPoint& pt ) 
{
	//calculate distance
	int distance = X ( ) * X ( ) + Y ( ) * Y ( ) ;
	distance = ( int ) sqrt ( (float) distance ) ;
	//return the distance
	return ( distance ) ;
}

//operators
//assignment
FSPoint& FSPoint::operator = ( FSPoint& pt ) 
{
	//copy x and y
	X ( ) = pt.X ( ) ;
	Y ( ) = pt.Y ( ) ;
	//return this point
	return ( *this ) ;
}

FSPoint& FSPoint::operator += ( FSPoint& pt ) 
{
	//add to x and y
	X ( ) += pt.X ( ) ;
	Y ( ) += pt.Y ( ) ;
	//return this point
	return ( *this ) ;
}

FSPoint& FSPoint::operator -= ( FSPoint& pt ) 
{
	//subtract x and y
	X ( ) -= pt.X ( ) ;
	Y ( ) -= pt.Y ( ) ;
	//return this point
	return ( *this ) ;
}

FSPoint& FSPoint::operator *= ( int scalar ) 
{
	//multiply by scalar
	X ( ) *= scalar ;
	Y ( ) *= scalar ;
	//return this point
	return ( *this ) ;
}

FSPoint& FSPoint::operator /= ( int scalar ) 
{
	//divide by scalar
	X ( ) /= scalar ;
	//return this point
	return ( *this ) ;
}

//unary
FSPoint FSPoint::operator - ( ) 
{
	//unary negativeness--multiply by -1
	FSPoint pt = ( *this ) ;
	pt *= -1 ;
	return ( pt ) ;
}

FSPoint FSPoint::operator + ( ) 
{
	//unary positiveness
	return ( *this ) ;
}

FSPoint FSPoint::operator + ( FSPoint& pt ) 
{
	//add points togetHer
	FSPoint result ;
	result = ( *this ) ;
	result += pt ;
	//return new point
	return ( result ) ;
}

FSPoint FSPoint::operator - ( FSPoint& pt ) 
{
	//subtract points
	FSPoint result ;
	result = ( *this ) ;
	result -= pt ;
	//return new point
	return ( result ) ;
}

FSPoint FSPoint::operator * ( int scalar ) 
{
	//multiply point by scalar
	FSPoint result ;
	result = ( *this ) ;
	result *= scalar ;
	//return new point
	return ( result ) ;
}

FSPoint FSPoint::operator / ( int scalar ) 
{
	//divide by scalar
	FSPoint result ;
	result = ( *this ) ;
	result /= scalar ;
	//return new point
	return ( result ) ;
}

//comparison
bool FSPoint::operator == ( FSPoint& pt ) 
{
	//check for equality
	return ( X ( ) == pt.X ( ) && Y ( ) == pt.Y ( ) ) ;
}

bool FSPoint::operator != ( FSPoint& pt ) 
{
	//check for inequality
	return ( !( ( *this ) == pt ) ) ;
}

FSPoint operator * ( int scalar , FSPoint& pt ) 
{
	//scalar*pt rather than pt*scalar, still the same idea...
	FSPoint result(pt);
	result *= scalar ;
	return ( result ) ;
}
