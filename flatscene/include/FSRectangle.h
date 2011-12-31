#ifndef __RECTANGLE_H__
#define __RECTANGLE_H__

#include "SDL.h"
#include "FSPoint.h"

//CRectangle--abstract an SDL_Rect
class FSRectangle  
{
private:
	Sint16 x,y,w,h;
public:
	//constructors--direct member assignment
	FSRectangle ( Sint16 x = 0 , Sint16 y = 0 , Sint16 w = 0 , Sint16 h = 0 ) ;
	//copy from SDL_Rect
	FSRectangle ( SDL_Rect rc ) ;
	//copy from SDL_Rect*
	FSRectangle ( SDL_Rect* prc ) ;
	//copy from another CRectangle
	FSRectangle ( FSRectangle& rc ) ;
	virtual ~FSRectangle ( ) ;

	//accessors for x, y, h, and w
	Sint16& X ( ) ;
	Sint16& Y ( ) ;
	Sint16& W ( ) ;
	Sint16& H ( ) ;

	//getters
	Sint16 getX() const;
	Sint16 getY() const;
	Sint16 getW() const;
	Sint16 getH() const;

	//setters
	void setX(Sint16 x);
	void setY(Sint16 y);
	void setW(Sint16 w);
	void setH(Sint16 h);

	//conversion operators
	//convert to SDL_Rect
	operator SDL_Rect ( ) ;

	//convert to CPoint
	operator FSPoint ( ) ;

	//set values for members
	FSRectangle& set ( Sint16 x , Sint16 y , Sint16 w , Sint16 h ) ;
	//copy member values from another CRectangle
	FSRectangle& copy ( FSRectangle& rc ) ;

	//set to an empty rectangle
	FSRectangle& setEmpty ( ) ;
	//check for emptiness
	bool IsEmpty ( ) ;

	//offset rectangle by coordinates or point
	FSRectangle& Offset ( Sint16 dx , Sint16 dy ) ;
	FSRectangle& Offset ( FSPoint& pt ) ;

	//move to a position, either coordinates or point
	FSRectangle& move ( Sint16 x , Sint16 y ) ;
	FSRectangle& move ( FSPoint& pt ) ;

	//intersect with another rectangle
	bool Intersect ( FSRectangle& rc ) ;
	//create union with another rectangle
	//CRectangle& Union ( CRectangle& rc ) ;

	//check if a point is within the rectangle
	bool Contains ( Sint16 x , Sint16 y ) ;
	bool Contains ( FSPoint& pt ) ;

	//assignment operators
	FSRectangle& operator = ( FSRectangle& rc ) ;
	FSRectangle& operator += ( FSPoint& pt ) ;
	FSRectangle& operator -= ( FSPoint& pt ) ;
	/*CRectangle& operator += ( CRectangle& rc ) ;
	CRectangle& operator -= ( CRectangle& rc ) ;

	//arithmetic operators
	CRectangle operator + ( CPoint& pt ) ;
	CRectangle operator - ( CPoint& pt ) ;*/
	FSRectangle operator + ( FSRectangle& rc ) ;
	FSRectangle operator - ( FSRectangle& rc ) ;

	//comparisons
	bool operator == ( FSRectangle& rc ) ;
	bool operator != ( FSRectangle& rc ) ;

	//clip or wrap points
	FSPoint Clip ( FSPoint pt ) ;
	FSPoint Wrap ( FSPoint pt ) ;
};

//add/subtract point and rectangle
FSRectangle operator + ( FSPoint& pt , FSRectangle& rc ) ;
FSRectangle operator - ( FSPoint& pt , FSRectangle& rc ) ;

#endif
