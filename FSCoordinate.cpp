#include "Coordinate.h"
#include <math.h>

//constructor
CCoordinate::CCoordinate ( int x , int y , int z ) : CPoint(x,y)
{
	//set the x and y of the Coordinate
	setZ( z );
}

CCoordinate::CCoordinate ( CCoordinate& coord ) 
{
	//set x and y of the Coordinate
	setX ( coord.getX ( ) ) ;
	setY ( coord.getY ( ) ) ;
	setZ ( coord.getZ ( ) ) ;
}


//destructor
CCoordinate::~CCoordinate ( ) 
{
	//do nothing
}

int& CCoordinate::Z ( )
{
	return ( m_z );
}

int CCoordinate::getZ() const {
	return (m_z);
}

void CCoordinate::setZ( int z) {
	m_z= z;
}

//setter
CCoordinate& CCoordinate::set ( int x , int y , int z ) 
{
	//set x and y
	X ( ) = x ;
	Y ( ) = y ;
	Z ( ) = z ;
	//return this Coordinate
	return ( *this ) ;
}

CCoordinate& CCoordinate::copy ( CCoordinate& coord ) 
{
	//copy x and y
	X ( ) = coord.X ( ) ;
	Y ( ) = coord.Y ( ) ;
	Z ( ) = coord. Z ( ) ;
	//return this Coordinate
	return ( *this ) ;
}

//move
CCoordinate& CCoordinate::move ( int dx , int dy , int dz ) 
{
	//move x and y
	X ( ) += dx ;
	Y ( ) += dy ;
	Z ( ) += dz ;
	//return this Coordinate
	return ( *this ) ;
}

CCoordinate& CCoordinate::add ( CCoordinate& coord ) 
{
	//add Coordinate to this Coordinate
	( *this ) += coord ;
	//return this Coordinate
	return ( *this ) ;
}

CCoordinate& CCoordinate::subtract ( CCoordinate& coord ) 
{
	//subtract Coordinate from this Coordinate
	( *this ) -= coord ;
	//return this Coordinate
	return ( *this ) ;
}

//scale
CCoordinate& CCoordinate::scale ( int scalar ) 
{
	//multiply this Coordinate by a scalar
	( *this ) *= scalar ;
	//return this Coordinate
	return ( *this ) ;
}

//distance
int CCoordinate::distance ( CCoordinate& coord ) 
{
	//calculate distance
	int distance = X ( ) * X ( ) + Y ( ) * Y ( ) + Z ( ) * Z ( ) ;
	distance = ( int ) sqrt ( (float) distance ) ;
	//return the distance
	return ( distance ) ;
}

//operators
//assignment
CCoordinate& CCoordinate::operator = ( CCoordinate& coord ) 
{
	//copy x and y
	X ( ) = coord.X ( ) ;
	Y ( ) = coord.Y ( ) ;
	Z ( ) = coord.Z();
	//return this Coordinate
	return ( *this ) ;
}

CCoordinate& CCoordinate::operator += ( CCoordinate& coord ) 
{
	//add to x and y
	X ( ) += coord.X ( ) ;
	Y ( ) += coord.Y ( ) ;
	Z () += coord.Z();
	//return this Coordinate
	return ( *this ) ;
}

CCoordinate& CCoordinate::operator -= ( CCoordinate& coord ) 
{
	//subtract x and y
	X ( ) -= coord.X ( ) ;
	Y ( ) -= coord.Y ( ) ;
	Z () -= coord.Z();
	//return this Coordinate
	return ( *this ) ;
}

CCoordinate& CCoordinate::operator += ( CPoint& pt ) 
{
	//add to x and y
	X ( ) += pt.X ( ) ;
	Y ( ) += pt.Y ( ) ;
	//return this Coordinate
	return ( *this ) ;
}

CCoordinate& CCoordinate::operator -= ( CPoint& pt ) 
{
	//subtract x and y
	X ( ) -= pt.X ( ) ;
	Y ( ) -= pt.Y ( ) ;
	//return this Coordinate
	return ( *this ) ;
}

CCoordinate& CCoordinate::operator *= ( int scalar ) 
{
	//multiply by scalar
	X ( ) *= scalar ;
	Y ( ) *= scalar ;
	Z ()*= scalar;
	//return this Coordinate
	return ( *this ) ;
}

CCoordinate& CCoordinate::operator /= ( int scalar ) 
{
	//divide by scalar
	X ( ) /= scalar ;
	Y() /= scalar;
	Z() /= scalar;
	//return this Coordinate
	return ( *this ) ;
}

//unary
CCoordinate CCoordinate::operator - ( ) 
{
	//unary negativeness--multiply by -1
	CCoordinate coord = ( *this ) ;
	coord *= -1 ;
	return ( coord ) ;
}

CCoordinate CCoordinate::operator + ( ) 
{
	//unary positiveness
	return ( *this ) ;
}

CCoordinate CCoordinate::operator + ( CCoordinate& coord ) 
{
	//add Coordinates togetHer
	CCoordinate result ;
	result = ( *this ) ;
	result += coord ;
	//return new Coordinate
	return ( result ) ;
}

CCoordinate CCoordinate::operator - ( CCoordinate& coord ) 
{
	//subtract Coordinates
	CCoordinate result ;
	result = ( *this ) ;
	result -= coord ;
	//return new Coordinate
	return ( result ) ;
}

CCoordinate CCoordinate::operator + ( CPoint& pt )  {
	CCoordinate result;
	result = (*this);
	result += pt;
	return result;
}
CCoordinate CCoordinate::operator - ( CPoint& pt )  {
	CCoordinate result;
	result = (*this);
	result -= pt;
	return result;
}

CCoordinate CCoordinate::operator * ( int scalar ) 
{
	//multiply Coordinate by scalar
	CCoordinate result ;
	result = ( *this ) ;
	result *= scalar ;
	//return new Coordinate
	return ( result ) ;
}

CCoordinate CCoordinate::operator / ( int scalar ) 
{
	//divide by scalar
	CCoordinate result ;
	result = ( *this ) ;
	result /= scalar ;
	//return new Coordinate
	return ( result ) ;
}

//comparison
bool CCoordinate::operator == ( CCoordinate& coord ) 
{
	//check for equality
	return ( X ( ) == coord.X ( ) && Y ( ) == coord.Y ( )  && Z()==coord.Z()) ;
}

bool CCoordinate::operator != ( CCoordinate& coord ) 
{
	//check for inequality
	return ( !( ( *this ) == coord ) ) ;
}

CCoordinate operator * ( int scalar , CCoordinate& coord ) 
{
	//scalar*coord rather than coord*scalar, still the same idea...
	CCoordinate result (coord);
	result *= scalar ;
	return ( result ) ;
}
