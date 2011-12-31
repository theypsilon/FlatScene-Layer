#include "FSCoordinate.h"
#include <math.h>

//constructor
FSCoordinate::FSCoordinate ( int x , int y , int z ) : FSPoint(x,y)
{
	//set the x and y of the Coordinate
	setZ( z );
}

FSCoordinate::FSCoordinate ( FSCoordinate& coord ) 
{
	//set x and y of the Coordinate
	setX ( coord.getX ( ) ) ;
	setY ( coord.getY ( ) ) ;
	setZ ( coord.getZ ( ) ) ;
}


//destructor
FSCoordinate::~FSCoordinate ( ) 
{
	//do nothing
}

int& FSCoordinate::Z ( )
{
	return ( m_z );
}

int FSCoordinate::getZ() const {
	return (m_z);
}

void FSCoordinate::setZ( int z) {
	m_z= z;
}

//setter
FSCoordinate& FSCoordinate::set ( int x , int y , int z ) 
{
	//set x and y
	X ( ) = x ;
	Y ( ) = y ;
	Z ( ) = z ;
	//return this Coordinate
	return ( *this ) ;
}

FSCoordinate& FSCoordinate::copy ( FSCoordinate& coord ) 
{
	//copy x and y
	X ( ) = coord.X ( ) ;
	Y ( ) = coord.Y ( ) ;
	Z ( ) = coord. Z ( ) ;
	//return this Coordinate
	return ( *this ) ;
}

//move
FSCoordinate& FSCoordinate::move ( int dx , int dy , int dz ) 
{
	//move x and y
	X ( ) += dx ;
	Y ( ) += dy ;
	Z ( ) += dz ;
	//return this Coordinate
	return ( *this ) ;
}

FSCoordinate& FSCoordinate::add ( FSCoordinate& coord ) 
{
	//add Coordinate to this Coordinate
	( *this ) += coord ;
	//return this Coordinate
	return ( *this ) ;
}

FSCoordinate& FSCoordinate::subtract ( FSCoordinate& coord ) 
{
	//subtract Coordinate from this Coordinate
	( *this ) -= coord ;
	//return this Coordinate
	return ( *this ) ;
}

//scale
FSCoordinate& FSCoordinate::scale ( int scalar ) 
{
	//multiply this Coordinate by a scalar
	( *this ) *= scalar ;
	//return this Coordinate
	return ( *this ) ;
}

//distance
int FSCoordinate::distance ( FSCoordinate& coord ) 
{
	//calculate distance
	int distance = X ( ) * X ( ) + Y ( ) * Y ( ) + Z ( ) * Z ( ) ;
	distance = ( int ) sqrt ( (float) distance ) ;
	//return the distance
	return ( distance ) ;
}

//operators
//assignment
FSCoordinate& FSCoordinate::operator = ( FSCoordinate& coord ) 
{
	//copy x and y
	X ( ) = coord.X ( ) ;
	Y ( ) = coord.Y ( ) ;
	Z ( ) = coord.Z();
	//return this Coordinate
	return ( *this ) ;
}

FSCoordinate& FSCoordinate::operator += ( FSCoordinate& coord ) 
{
	//add to x and y
	X ( ) += coord.X ( ) ;
	Y ( ) += coord.Y ( ) ;
	Z () += coord.Z();
	//return this Coordinate
	return ( *this ) ;
}

FSCoordinate& FSCoordinate::operator -= ( FSCoordinate& coord ) 
{
	//subtract x and y
	X ( ) -= coord.X ( ) ;
	Y ( ) -= coord.Y ( ) ;
	Z () -= coord.Z();
	//return this Coordinate
	return ( *this ) ;
}

FSCoordinate& FSCoordinate::operator += ( FSPoint& pt ) 
{
	//add to x and y
	X ( ) += pt.X ( ) ;
	Y ( ) += pt.Y ( ) ;
	//return this Coordinate
	return ( *this ) ;
}

FSCoordinate& FSCoordinate::operator -= ( FSPoint& pt ) 
{
	//subtract x and y
	X ( ) -= pt.X ( ) ;
	Y ( ) -= pt.Y ( ) ;
	//return this Coordinate
	return ( *this ) ;
}

FSCoordinate& FSCoordinate::operator *= ( int scalar ) 
{
	//multiply by scalar
	X ( ) *= scalar ;
	Y ( ) *= scalar ;
	Z ()*= scalar;
	//return this Coordinate
	return ( *this ) ;
}

FSCoordinate& FSCoordinate::operator /= ( int scalar ) 
{
	//divide by scalar
	X ( ) /= scalar ;
	Y() /= scalar;
	Z() /= scalar;
	//return this Coordinate
	return ( *this ) ;
}

//unary
FSCoordinate FSCoordinate::operator - ( ) 
{
	//unary negativeness--multiply by -1
	FSCoordinate coord = ( *this ) ;
	coord *= -1 ;
	return ( coord ) ;
}

FSCoordinate FSCoordinate::operator + ( ) 
{
	//unary positiveness
	return ( *this ) ;
}

FSCoordinate FSCoordinate::operator + ( FSCoordinate& coord ) 
{
	//add Coordinates togetHer
	FSCoordinate result ;
	result = ( *this ) ;
	result += coord ;
	//return new Coordinate
	return ( result ) ;
}

FSCoordinate FSCoordinate::operator - ( FSCoordinate& coord ) 
{
	//subtract Coordinates
	FSCoordinate result ;
	result = ( *this ) ;
	result -= coord ;
	//return new Coordinate
	return ( result ) ;
}

FSCoordinate FSCoordinate::operator + ( FSPoint& pt )  {
	FSCoordinate result;
	result = (*this);
	result += pt;
	return result;
}
FSCoordinate FSCoordinate::operator - ( FSPoint& pt )  {
	FSCoordinate result;
	result = (*this);
	result -= pt;
	return result;
}

FSCoordinate FSCoordinate::operator * ( int scalar ) 
{
	//multiply Coordinate by scalar
	FSCoordinate result ;
	result = ( *this ) ;
	result *= scalar ;
	//return new Coordinate
	return ( result ) ;
}

FSCoordinate FSCoordinate::operator / ( int scalar ) 
{
	//divide by scalar
	FSCoordinate result ;
	result = ( *this ) ;
	result /= scalar ;
	//return new Coordinate
	return ( result ) ;
}

//comparison
bool FSCoordinate::operator == ( FSCoordinate& coord ) 
{
	//check for equality
	return ( X ( ) == coord.X ( ) && Y ( ) == coord.Y ( )  && Z()==coord.Z()) ;
}

bool FSCoordinate::operator != ( FSCoordinate& coord ) 
{
	//check for inequality
	return ( !( ( *this ) == coord ) ) ;
}

FSCoordinate operator * ( int scalar , FSCoordinate& coord ) 
{
	//scalar*coord rather than coord*scalar, still the same idea...
	FSCoordinate result (coord);
	result *= scalar ;
	return ( result ) ;
}
