// Rect.cpp: implementation of the CRectangle class.
#include "FSRectangle.h"

FSRectangle::FSRectangle ( Sint16 x , Sint16 y , Sint16 w , Sint16 h ) 
{
	set ( x , y , w , h ) ;
}

FSRectangle::FSRectangle ( SDL_Rect rc ) 
{
	set ( rc.x , rc.y , rc.w , rc.h ) ;
}

FSRectangle::FSRectangle ( SDL_Rect* prc ) 
{
	set ( prc->x , prc->y , prc->w , prc->h ) ;
}

FSRectangle::FSRectangle ( FSRectangle& rc ) 
{
	( *this ) = rc ;
}

FSRectangle::~FSRectangle ( ) 
{
}

Sint16& FSRectangle::X ( ) 
{
	return ( x ) ;
}

Sint16& FSRectangle::Y ( ) 
{
	return ( y ) ;
}

Sint16& FSRectangle::W ( ) 
{
	return ( w ) ;
}

Sint16& FSRectangle::H ( ) 
{
	return ( h ) ;
}

//getters
Sint16 FSRectangle::getX() const
{
	return(x);
}

Sint16 FSRectangle::getY() const
{
	return(y);
}

Sint16 FSRectangle::getW() const
{
	return(w);
}

Sint16 FSRectangle::getH() const
{
	return(h);
}

//setters
void FSRectangle::setX(Sint16 x)
{
	this->x=x;
}

void FSRectangle::setY(Sint16 y)
{
	this->y=y;
}

void FSRectangle::setW(Sint16 w)
{
	this->w=w;
}

void FSRectangle::setH(Sint16 h)
{
	this->h=h;
}

FSRectangle::operator SDL_Rect ( ) 
{
	SDL_Rect m_rect = { x,y,w,h };
	return ( m_rect ) ;
}

FSRectangle::operator FSPoint ( ) 
{
	FSPoint pt ;
	pt.set ( X ( ) , Y ( ) ) ;
	return ( pt ) ;
}

FSRectangle& FSRectangle::set ( Sint16 x , Sint16 y , Sint16 w , Sint16 h ) 
{
	X ( ) = x ;
	Y ( ) = y ;
	W ( ) = w ;
	H ( ) = h ;
	return ( *this ) ;
}

FSRectangle& FSRectangle::copy ( FSRectangle& rc ) 
{
	( *this ) = rc ;
	return ( *this ) ;
}

FSRectangle& FSRectangle::setEmpty ( ) 
{
	set ( 0 , 0 , 0 , 0 ) ;
	return ( *this ) ;
}

bool FSRectangle::IsEmpty ( ) 
{
	return ( W ( ) == 0 && H ( ) == 0 ) ;
}

FSRectangle& FSRectangle::Offset ( Sint16 dx , Sint16 dy ) 
{
	X ( ) += dx ;
	Y ( ) += dy ;
	return ( *this ) ;
}

FSRectangle& FSRectangle::Offset ( FSPoint& pt ) 
{
	Offset ( pt.X ( ) , pt.Y ( ) ) ;
	return ( *this ) ;
}

FSRectangle& FSRectangle::move ( Sint16 x , Sint16 y ) 
{
	X ( ) = x ;
	Y ( ) = y ;
	return ( *this ) ;
}

FSRectangle& FSRectangle::move ( FSPoint& pt ) 
{
	move ( pt.X ( ) , pt.Y ( ) ) ;
	return ( *this ) ;
}

bool FSRectangle::Intersect ( FSRectangle& rc ) 
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

bool FSRectangle::Contains ( Sint16 x , Sint16 y ) 
{
	x -= X ( ) ;
	y -= Y ( ) ;
	return ( x >= 0 && y >= 0 && x < W ( ) && y < H ( ) ) ;
}

bool FSRectangle::Contains ( FSPoint& pt ) 
{
	return ( Contains ( pt.X ( ) , pt.Y ( ) ) ) ;
}

FSRectangle& FSRectangle::operator = ( FSRectangle& rc ) 
{
	X ( ) = rc.X ( ) ;
	Y ( ) = rc.Y ( ) ;
	W ( ) = rc.W ( ) ;
	H ( ) = rc.H ( ) ;
	return ( *this ) ;
}

FSRectangle& FSRectangle::operator += ( FSPoint& pt ) 
{
	X ( ) += pt.X ( ) ;
	Y ( ) += pt.Y ( ) ;
	return ( *this ) ;
}

FSRectangle& FSRectangle::operator -= ( FSPoint& pt ) 
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
FSRectangle FSRectangle::operator + ( FSRectangle& rc ) 
{
	int left1 , top1 , right1, bottom1 ;
	int left2 , top2 , right2, bottom2 ;

	FSRectangle result ;
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

FSRectangle FSRectangle::operator - ( FSRectangle& rc ) 
{
	int left1 , top1 , right1, bottom1 ;
	int left2 , top2 , right2, bottom2 ;

	FSRectangle result ;
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

bool FSRectangle::operator == ( FSRectangle& rc ) 
{
	return ( X ( ) == rc.X ( ) && Y ( ) == rc.Y ( ) && W ( ) == rc.W ( ) && H ( ) == rc.H ( ) ) ;
}

bool FSRectangle::operator != ( FSRectangle& rc ) 
{
	return ( !( ( *this ) == rc ) );
}

FSRectangle operator + ( FSPoint& pt , FSRectangle& rc ) 
{
	FSRectangle result = rc ;
	result += pt ;
	return ( result ) ;
}

FSRectangle operator - ( FSPoint& pt , FSRectangle& rc ) 
{
	FSRectangle result = rc ;
	result -= pt ;
	return ( result ) ;
}

FSPoint FSRectangle::Clip ( FSPoint pt ) 
{
	if ( pt.X ( ) < X ( ) ) pt.X ( ) = X ( ) ;
	if ( pt.Y ( ) < Y ( ) ) pt.Y ( ) = Y ( ) ;
	if ( pt.X ( ) >= X ( ) + W ( ) ) pt.X ( ) = X ( ) + W ( ) - 1 ;
	if ( pt.Y ( ) >= Y ( ) + H ( ) ) pt.Y ( ) = Y ( ) + H ( ) - 1 ;
	return ( pt ) ;
}

FSPoint FSRectangle::Wrap ( FSPoint pt ) 
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

