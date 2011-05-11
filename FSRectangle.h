#ifndef __RECTANGLE_H__
#define __RECTANGLE_H__

#include "SDL.h"
#include "FSPoint.h"

//CRectangle--abstract an SDL_Rect
class CRectangle  
{
private:
	Sint16 x,y,w,h;
public:
	//constructors--direct member assignment
	CRectangle ( Sint16 x = 0 , Sint16 y = 0 , Sint16 w = 0 , Sint16 h = 0 ) ;
	//copy from SDL_Rect
	CRectangle ( SDL_Rect rc ) ;
	//copy from SDL_Rect*
	CRectangle ( SDL_Rect* prc ) ;
	//copy from another CRectangle
	CRectangle ( CRectangle& rc ) ;
	virtual ~CRectangle ( ) ;

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
	operator CPoint ( ) ;

	//set values for members
	CRectangle& set ( Sint16 x , Sint16 y , Sint16 w , Sint16 h ) ;
	//copy member values from another CRectangle
	CRectangle& copy ( CRectangle& rc ) ;

	//set to an empty rectangle
	CRectangle& setEmpty ( ) ;
	//check for emptiness
	bool IsEmpty ( ) ;

	//offset rectangle by coordinates or point
	CRectangle& Offset ( Sint16 dx , Sint16 dy ) ;
	CRectangle& Offset ( CPoint& pt ) ;

	//move to a position, either coordinates or point
	CRectangle& move ( Sint16 x , Sint16 y ) ;
	CRectangle& move ( CPoint& pt ) ;

	//intersect with another rectangle
	bool Intersect ( CRectangle& rc ) ;
	//create union with another rectangle
	//CRectangle& Union ( CRectangle& rc ) ;

	//check if a point is within the rectangle
	bool Contains ( Sint16 x , Sint16 y ) ;
	bool Contains ( CPoint& pt ) ;

	//assignment operators
	CRectangle& operator = ( CRectangle& rc ) ;
	CRectangle& operator += ( CPoint& pt ) ;
	CRectangle& operator -= ( CPoint& pt ) ;
	/*CRectangle& operator += ( CRectangle& rc ) ;
	CRectangle& operator -= ( CRectangle& rc ) ;

	//arithmetic operators
	CRectangle operator + ( CPoint& pt ) ;
	CRectangle operator - ( CPoint& pt ) ;*/
	CRectangle operator + ( CRectangle& rc ) ;
	CRectangle operator - ( CRectangle& rc ) ;

	//comparisons
	bool operator == ( CRectangle& rc ) ;
	bool operator != ( CRectangle& rc ) ;

	//clip or wrap points
	CPoint Clip ( CPoint pt ) ;
	CPoint Wrap ( CPoint pt ) ;
};

//add/subtract point and rectangle
CRectangle operator + ( CPoint& pt , CRectangle& rc ) ;
CRectangle operator - ( CPoint& pt , CRectangle& rc ) ;

#endif
