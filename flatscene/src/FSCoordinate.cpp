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
//distance
int FSCoordinate::distance ( FSCoordinate& coord ) 
{
	//calculate distance
	int distance = X ( ) * X ( ) + Y ( ) * Y ( ) + Z ( ) * Z ( ) ;
	distance = ( int ) sqrt ( (float) distance ) ;
	//return the distance
	return ( distance ) ;
}