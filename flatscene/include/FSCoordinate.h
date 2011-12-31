#ifndef  __COORDINATE_H__
#define __COORDINATE_H__

#include "FSPoint.h"

class FSCoordinate : public FSPoint {
private:
	int m_z;
public:
	FSCoordinate(int x = 0,int y  = 0, int z = 0);
	FSCoordinate(FSCoordinate& coord);

	int& Z();
	inline int getZ() const ;
	void setZ(int z) ;

	FSCoordinate& set(int x,int y,int z);
	int FSCoordinate::distance ( FSCoordinate& coord );
};

#endif