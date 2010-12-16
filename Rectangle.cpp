// Rect.cpp: implementation of the CRectangle class.
#include "Rectangle.h"

CRectangle::CRectangle ( Sint16 x , Sint16 y , Sint16 w , Sint16 h ) 
{
	set ( x , y , w , h ) ;
}

CRectangle::CRectangle ( SDL_Rect rc ) 
{
	set ( rc.x , rc.y , rc.w , rc.h ) ;
}

CRectangle::CRectangle ( SDL_Rect* prc ) 
{
	set ( prc->x , prc->y , prc->w , prc->h ) ;
}

CRectangle::CRectangle ( CRectangle& rc ) 
{
	( *this ) = rc ;
}

CRectangle::~CRectangle ( ) 
{
}

Sint16& CRectangle::X ( ) 
{
	return ( x ) ;
}

Sint16& CRectangle::Y ( ) 
{
	return ( y ) ;
}

Sint16& CRectangle::W ( ) 
{
	return ( w ) ;
}

Sint16& CRectangle::H ( ) 
{
	return ( h ) ;
}

//getters
Sint16 CRectangle::getX() const
{
	return(x);
}

Sint16 CRectangle::getY() const
{
	return(y);
}

Sint16 CRectangle::getW() const
{
	return(w);
}

Sint16 CRectangle::getH() const
{
	return(h);
}

//setters
void CRectangle::setX(Sint16 x)
{
	this->x=x;
}

void CRectangle::setY(Sint16 y)
{
	this->y=y;
}

void CRectangle::setW(Sint16 w)
{
	this->w=w;
}

void CRectangle::setH(Sint16 h)
{
	this->h=h;
}

CRectangle::operator SDL_Rect ( ) 
{
	SDL_Rect m_rect = { x,y,w,h };
	return ( m_rect ) ;
}

CRectangle::operator CPoint ( ) 
{
	CPoint pt ;
	pt.set ( X ( ) , Y ( ) ) ;
	return ( pt ) ;
}

CRectangle& CRectangle::set ( Sint16 x , Sint16 y , Sint16 w , Sint16 h ) 
{
	X ( ) = x ;
	Y ( ) = y ;
	W ( ) = w ;
	H ( ) = h ;
	return ( *this ) ;
}

CRectangle& CRectangle::copy ( CRectangle& rc ) 
{
	( *this ) = rc ;
	return ( *this ) ;
}

CRectangle& CRectangle::setEmpty ( ) 
{
	set ( 0 , 0 , 0 , 0 ) ;
	return ( *this ) ;
}

bool CRectangle::IsEmpty ( ) 
{
	return ( W ( ) == 0 && H ( ) == 0 ) ;
}

CRectangle& CRectangle::Offset ( Sint16 dx , Sint16 dy ) 
{
	X ( ) += dx ;
	Y ( ) += dy ;
	return ( *this ) ;
}

CRectangle& CRectangle::Offset ( CPoint& pt ) 
{
	Offset ( pt.X ( ) , pt.Y ( ) ) ;
	return ( *this ) ;
}

CRectangle& CRectangle::move ( Sint16 x , Sint16 y ) 
{
	X ( ) = x ;
	Y ( ) = y ;
	return ( *this ) ;
}

CRectangle& CRectangle::move ( CPoint& pt ) 
{
	move ( pt.X ( ) , pt.Y ( ) ) ;
	return ( *this ) ;
}

bool CRectangle::Intersect ( CRectangle& rc ) 
{
	if ((X() >= rc.X() && X() <= rc.W() && Y() >= rc.Y() && Y() <= rc.H()) ||
		(W() >= rc.X() && W() <= rc.W() && Y() >= rc.Y() && Y() <= rc.H()) ||
		(X() >= rc.X() && X() <= rc.W() && H() >= rc.Y() && H() <= rc.H()) ||
		(W() >= rc.X() && W() <= rc.W() && H() >= rc.Y() && H() <= rc.H()))
		return true;
	return false;
}
/*
CRectangle& CRectangle::Union ( CRectangle& rc ) 
{
	( *this ) += rc ;
	return ( *this ) ;
}*/

bool CRectangle::Contains ( Sint16 x , Sint16 y ) 
{
	x -= X ( ) ;
	y -= Y ( ) ;
	return ( x >= 0 && y >= 0 && x < W ( ) && y < H ( ) ) ;
}

bool CRectangle::Contains ( CPoint& pt ) 
{
	return ( Contains ( pt.X ( ) , pt.Y ( ) ) ) ;
}

CRectangle& CRectangle::operator = ( CRectangle& rc ) 
{
	X ( ) = rc.X ( ) ;
	Y ( ) = rc.Y ( ) ;
	W ( ) = rc.W ( ) ;
	H ( ) = rc.H ( ) ;
	return ( *this ) ;
}

CRectangle& CRectangle::operator += ( CPoint& pt ) 
{
	X ( ) += pt.X ( ) ;
	Y ( ) += pt.Y ( ) ;
	return ( *this ) ;
}

CRectangle& CRectangle::operator -= ( CPoint& pt ) 
{
	X ( ) -= pt.X ( ) ;
	Y ( ) -= pt.Y ( ) ;
	return ( *this ) ;
}
/*
CRectangle& CRectangle::operator += ( CRectangle& rc ) 
{
	( *this ) = ( *this ) + rc ;
	return ( *this ) ;
}

CRectangle& CRectangle::operator -= ( CRectangle& rc ) 
{
	( *this ) = ( *this ) - rc ;
	return ( *this ) ;
}

CRectangle CRectangle::operator + ( CPoint& pt ) 
{
	CRectangle result = ( *this ) ;
	result += pt ;
	return ( result ) ;
}

CRectangle CRectangle::operator - ( CPoint& pt ) 
{
	CRectangle result = ( *this ) ;
	result -= pt ;
	return ( result ) ;
}
*/
CRectangle CRectangle::operator + ( CRectangle& rc ) 
{
	int left1 , top1 , right1, bottom1 ;
	int left2 , top2 , right2, bottom2 ;

	CRectangle result ;
	result.setEmpty ( ) ;

	left1 = X ( ) ;
	top1 = Y ( ) ;
	right1 = X ( ) + W ( ) ;
	bottom1 = Y ( ) + H ( ) ;

	left2 = rc.X ( ) ;
	top2 = rc.Y ( ) ;
	right2 = rc.X ( ) + rc.W ( ) ;
	bottom2 = rc.Y ( ) + rc.H ( ) ;

	if ( left1 > left2 ) left1 = left2 ;
	if ( top1 > top2 ) top1 = top2 ;
	if ( right1 < right2 ) right1 = right2 ;
	if ( bottom1 < bottom2 ) bottom1 = bottom2 ;

	right1 -= left1 ;
	bottom1 -= top1 ;

	if ( right1 > 0 && bottom1 > 0 )
	{
		result.X ( ) = left1 ;
		result.Y ( ) = top1 ;
		result.W ( ) = right1 ;
		result.H ( ) = bottom1 ;
	}
		
	return ( result ) ;
}

CRectangle CRectangle::operator - ( CRectangle& rc ) 
{
	int left1 , top1 , right1, bottom1 ;
	int left2 , top2 , right2, bottom2 ;

	CRectangle result ;
	result.setEmpty ( ) ;

	left1 = X ( ) ;
	top1 = Y ( ) ;
	right1 = X ( ) + W ( ) ;
	bottom1 = Y ( ) + H ( ) ;

	left2 = rc.X ( ) ;
	top2 = rc.Y ( ) ;
	right2 = rc.X ( ) + rc.W ( ) ;
	bottom2 = rc.Y ( ) + rc.H ( ) ;

	if ( left1 < left2 ) left1 = left2 ;
	if ( top1 < top2 ) top1 = top2 ;
	if ( right1 > right2 ) right1 = right2 ;
	if ( bottom1 > bottom2 ) bottom1 = bottom2 ;

	right1 -= left1 ;
	bottom1 -= top1 ;

	if ( right1 > 0 && bottom1 > 0 )
	{
		result.X ( ) = left1 ;
		result.Y ( ) = top1 ;
		result.W ( ) = right1 ;
		result.H ( ) = bottom1 ;
	}
		
	return ( result ) ;
}

bool CRectangle::operator == ( CRectangle& rc ) 
{
	return ( X ( ) == rc.X ( ) && Y ( ) == rc.Y ( ) && W ( ) == rc.W ( ) && H ( ) == rc.H ( ) ) ;
}

bool CRectangle::operator != ( CRectangle& rc ) 
{
	return ( !( ( *this ) == rc ) );
}

CRectangle operator + ( CPoint& pt , CRectangle& rc ) 
{
	CRectangle result = rc ;
	result += pt ;
	return ( result ) ;
}

CRectangle operator - ( CPoint& pt , CRectangle& rc ) 
{
	CRectangle result = rc ;
	result -= pt ;
	return ( result ) ;
}

CPoint CRectangle::Clip ( CPoint pt ) 
{
	if ( pt.X ( ) < X ( ) ) pt.X ( ) = X ( ) ;
	if ( pt.Y ( ) < Y ( ) ) pt.Y ( ) = Y ( ) ;
	if ( pt.X ( ) >= X ( ) + W ( ) ) pt.X ( ) = X ( ) + W ( ) - 1 ;
	if ( pt.Y ( ) >= Y ( ) + H ( ) ) pt.Y ( ) = Y ( ) + H ( ) - 1 ;
	return ( pt ) ;
}

CPoint CRectangle::Wrap ( CPoint pt ) 
{
	if ( IsEmpty ( ) )  
		pt.set(0,0); 
	else {
		while ( pt.X ( ) < X ( ) ) pt.X ( ) += W ( ) ;
		while ( pt.Y ( ) < Y ( ) ) pt.Y ( ) += H ( ) ;
		while ( pt.X ( ) >= X ( ) + W ( ) ) pt.X ( ) -= W ( ) ;
		while ( pt.Y ( ) >= Y ( ) + H ( ) ) pt.Y ( ) -= H ( ) ;
	}
	return ( pt ) ;
}

