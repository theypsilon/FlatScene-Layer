//sentinel
#ifndef  __COORDINATE_H__
#define __COORDINATE_H__

#include "FSPoint.h"

//point class
class FSCoordinate : public FSPoint{
private:
	int m_z ;
public:
	//constructor
	FSCoordinate ( int x = 0 , int y  = 0 , int z = 0) ;
	FSCoordinate ( FSCoordinate& coord ) ;

	//destructor
	virtual ~FSCoordinate ( ) ;

	int& Z ( ) ;

	inline int getZ ( ) const ;

	void setZ ( int z ) ;


	//setter
	FSCoordinate& set ( int x , int y , int z ) ;
	FSCoordinate& copy ( FSCoordinate& coord ) ;

	//move
	FSCoordinate& move ( int dx , int dy , int dz ) ;
	FSCoordinate& add ( FSCoordinate& coord ) ;
	FSCoordinate& subtract ( FSCoordinate& coord ) ;
	
	//scale
	FSCoordinate& scale ( int scalar ) ;

	//distance
	int distance ( FSCoordinate& coord ) ;

	//operators
	//assignment
	FSCoordinate& operator = ( FSCoordinate& coord ) ;
	FSCoordinate& operator += ( FSCoordinate& coord ) ;
	FSCoordinate& operator -= ( FSCoordinate& coord ) ;
	FSCoordinate& operator += ( FSPoint& pt ) ;
	FSCoordinate& operator -= ( FSPoint& pt ) ;
	FSCoordinate& operator *= ( int scalar ) ;
	FSCoordinate& operator /= ( int scalar ) ;

	//unary
	FSCoordinate operator - ( ) ;
	FSCoordinate operator + ( ) ;

	FSCoordinate operator + ( FSCoordinate& coord ) ;
	FSCoordinate operator - ( FSCoordinate& coord ) ;
	FSCoordinate operator + ( FSPoint& pt ) ;
	FSCoordinate operator - ( FSPoint& pt ) ;
	FSCoordinate operator * ( int scalar ) ;
	FSCoordinate operator / ( int scalar ) ;

	//comparison
	bool operator == ( FSCoordinate& coord ) ;
	bool operator != ( FSCoordinate& coord ) ;

};

FSCoordinate operator * ( int scalar , FSCoordinate& coord ) ;

//sentinel
#endif