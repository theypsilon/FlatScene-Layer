//sentinel
#ifndef  __COORDINATE_H__
#define __COORDINATE_H__

#include "Point.h"

//point class
class CCoordinate : public CPoint{
private:
	int m_z ;
public:
	//constructor
	CCoordinate ( int x = 0 , int y  = 0 , int z = 0) ;
	CCoordinate ( CCoordinate& coord ) ;

	//destructor
	virtual ~CCoordinate ( ) ;

	int& Z ( ) ;

	inline int getZ ( ) const ;

	void setZ ( int z ) ;


	//setter
	CCoordinate& set ( int x , int y , int z ) ;
	CCoordinate& copy ( CCoordinate& coord ) ;

	//move
	CCoordinate& move ( int dx , int dy , int dz ) ;
	CCoordinate& add ( CCoordinate& coord ) ;
	CCoordinate& subtract ( CCoordinate& coord ) ;
	
	//scale
	CCoordinate& scale ( int scalar ) ;

	//distance
	int distance ( CCoordinate& coord ) ;

	//operators
	//assignment
	CCoordinate& operator = ( CCoordinate& coord ) ;
	CCoordinate& operator += ( CCoordinate& coord ) ;
	CCoordinate& operator -= ( CCoordinate& coord ) ;
	CCoordinate& operator += ( CPoint& pt ) ;
	CCoordinate& operator -= ( CPoint& pt ) ;
	CCoordinate& operator *= ( int scalar ) ;
	CCoordinate& operator /= ( int scalar ) ;

	//unary
	CCoordinate operator - ( ) ;
	CCoordinate operator + ( ) ;

	CCoordinate operator + ( CCoordinate& coord ) ;
	CCoordinate operator - ( CCoordinate& coord ) ;
	CCoordinate operator + ( CPoint& pt ) ;
	CCoordinate operator - ( CPoint& pt ) ;
	CCoordinate operator * ( int scalar ) ;
	CCoordinate operator / ( int scalar ) ;

	//comparison
	bool operator == ( CCoordinate& coord ) ;
	bool operator != ( CCoordinate& coord ) ;

};

CCoordinate operator * ( int scalar , CCoordinate& coord ) ;

//sentinel
#endif