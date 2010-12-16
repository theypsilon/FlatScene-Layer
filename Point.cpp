#include "Point.h"
#include <math.h>

//constructor
CPoint::CPoint ( int x , int y ) 
{
	//set the x and y of the point
	set ( x , y ) ;
}

CPoint::CPoint ( CPoint& pt ) 
{
	//set x and y of the point
	setX ( pt.getX ( ) ) ;
	setY ( pt.getY ( ) ) ;
}


//destructor
CPoint::~CPoint ( ) 
{
	//do nothing
}

//properties
int& CPoint::X ( ) 
{
	//return x componEnt
	return ( m_x ) ;
}

int& CPoint::Y ( ) 
{
	//return y componEnt
	return ( m_y ) ;
}

int CPoint::getX ( ) const
{
	//return x componEnt
	return ( m_x ) ;
}

int CPoint::getY ( ) const
{
	//return y componEnt
	return ( m_y ) ;
}

void CPoint::setX ( int x ) 
{
	//assign new value to x componEnt
	m_x = x ;
}

void CPoint::setY ( int y ) 
{
	//assign new value to y componEnt
	m_y = y ;
}

//setter
CPoint& CPoint::set ( int x , int y ) 
{
	//set x and y
	X ( ) = x ;
	Y ( ) = y ;
	//return this point
	return ( *this ) ;
}

CPoint& CPoint::copy ( CPoint& pt ) 
{
	//copy x and y
	X ( ) = pt.X ( ) ;
	Y ( ) = pt.Y ( ) ;
	//return this point
	return ( *this ) ;
}

//move
CPoint& CPoint::move ( int dx , int dy ) 
{
	//move x and y
	X ( ) += dx ;
	Y ( ) += dy ;
	//return this point
	return ( *this ) ;
}

CPoint& CPoint::add ( CPoint& pt ) 
{
	//add point to this point
	( *this ) += pt ;
	//return this point
	return ( *this ) ;
}

CPoint& CPoint::subtract ( CPoint& pt ) 
{
	//subtract point from this point
	( *this ) -= pt ;
	//return this point
	return ( *this ) ;
}

//scale
CPoint& CPoint::scale ( int scalar ) 
{
	//multiply this point by a scalar
	( *this ) *= scalar ;
	//return this point
	return ( *this ) ;
}

//distance
int CPoint::distance ( CPoint& pt ) 
{
	//calculate distance
	int distance = X ( ) * X ( ) + Y ( ) * Y ( ) ;
	distance = ( int ) sqrt ( (float) distance ) ;
	//return the distance
	return ( distance ) ;
}

//operators
//assignment
CPoint& CPoint::operator = ( CPoint& pt ) 
{
	//copy x and y
	X ( ) = pt.X ( ) ;
	Y ( ) = pt.Y ( ) ;
	//return this point
	return ( *this ) ;
}

CPoint& CPoint::operator += ( CPoint& pt ) 
{
	//add to x and y
	X ( ) += pt.X ( ) ;
	Y ( ) += pt.Y ( ) ;
	//return this point
	return ( *this ) ;
}

CPoint& CPoint::operator -= ( CPoint& pt ) 
{
	//subtract x and y
	X ( ) -= pt.X ( ) ;
	Y ( ) -= pt.Y ( ) ;
	//return this point
	return ( *this ) ;
}

CPoint& CPoint::operator *= ( int scalar ) 
{
	//multiply by scalar
	X ( ) *= scalar ;
	Y ( ) *= scalar ;
	//return this point
	return ( *this ) ;
}

CPoint& CPoint::operator /= ( int scalar ) 
{
	//divide by scalar
	X ( ) /= scalar ;
	//return this point
	return ( *this ) ;
}

//unary
CPoint CPoint::operator - ( ) 
{
	//unary negativeness--multiply by -1
	CPoint pt = ( *this ) ;
	pt *= -1 ;
	return ( pt ) ;
}

CPoint CPoint::operator + ( ) 
{
	//unary positiveness
	return ( *this ) ;
}

CPoint CPoint::operator + ( CPoint& pt ) 
{
	//add points togetHer
	CPoint result ;
	result = ( *this ) ;
	result += pt ;
	//return new point
	return ( result ) ;
}

CPoint CPoint::operator - ( CPoint& pt ) 
{
	//subtract points
	CPoint result ;
	result = ( *this ) ;
	result -= pt ;
	//return new point
	return ( result ) ;
}

CPoint CPoint::operator * ( int scalar ) 
{
	//multiply point by scalar
	CPoint result ;
	result = ( *this ) ;
	result *= scalar ;
	//return new point
	return ( result ) ;
}

CPoint CPoint::operator / ( int scalar ) 
{
	//divide by scalar
	CPoint result ;
	result = ( *this ) ;
	result /= scalar ;
	//return new point
	return ( result ) ;
}

//comparison
bool CPoint::operator == ( CPoint& pt ) 
{
	//check for equality
	return ( X ( ) == pt.X ( ) && Y ( ) == pt.Y ( ) ) ;
}

bool CPoint::operator != ( CPoint& pt ) 
{
	//check for inequality
	return ( !( ( *this ) == pt ) ) ;
}

CPoint operator * ( int scalar , CPoint& pt ) 
{
	//scalar*pt rather than pt*scalar, still the same idea...
	CPoint result(pt);
	result *= scalar ;
	return ( result ) ;
}
