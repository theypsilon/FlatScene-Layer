#include "FSSprite.h"
#include "FSScreen.h"

CSprite::CSprite ( SCanvas pSurface, CPoint* zerocpSource) : CImage(pSurface) {

	if (zerocpSource==NULL) 
		cpoint =new CPoint(0,0);
	else
		cpoint = zerocpSource;

}

CSprite::~CSprite ( ) {

	vector<RectArea*>::iterator iter ;
	RectArea* area ;
	while ( !areas.empty ( ) ) {
		iter = areas.begin ( ) ;
		area = *iter ;
		RectArea::iterator iter2 ;
		CRectangle* rect;
		while ( !area->empty() ) {
			iter2 = area->begin();
			rect = *iter2;
			area->erase( iter2);
			delete rect;
		}
			
		areas.erase (iter) ;
		delete area ;
	}
	if ( cpoint != NULL ) {
		delete cpoint;
		cpoint = NULL;
	}

	if (m_pSurface.sdl_surf) {
		SDL_FreeSurface(m_pSurface.sdl_surf);
	}

	if (m_pSurface.h != 0 || m_pSurface.w !=0)
		glDeleteTextures( 1, &(m_pSurface.tex) );

	clearSurface();
	
}

CImage* CSprite::getImage ( ) {
	return ( (CImage*) this ) ;
}

void CSprite::put (CPoint& ptDst ,Uint8 flags) {
	if (flags & 0x001) {
		ptDst.Y() -= cpoint->Y();
		ptDst.X() -= getImage()->getWidth() - cpoint->X();
	} else {
		ptDst -= *cpoint;
	}

	CImage::put(ptDst,flags);
}
//TODO

void CSprite::setName (const char *newName) {
	name=newName;
}

string CSprite::getName() {
	return name;
}

int CSprite::addRect(int area,CRectangle* rect) {
	areas[area]->push_back(rect);
	return (areas[area]->size()-1);
}

int CSprite::addArea(RectArea* area) {
	areas.push_back(area);
	return (areas.size()-1);
}


RectArea* CSprite::getArea (int n ) 
{
#ifdef VECTOR_COMP
	if ((n<0) || (n >= areas.size()))
		return NULL;
#endif
	return (areas[n]) ;
}

CRectangle* CSprite::getRect (int n ,int m) 
{
#ifdef VECTOR_COMP
	if ((n<0) || (n >= areas.size()) || (m<0) || (m >= areas[n]->size()))
		return NULL;
#endif
	return ((*areas[n])[m]) ;
}

CPoint* CSprite::getCenter() {
	return cpoint;
}

void CSprite::replaceCenter(CPoint *c) {
	delete cpoint;
	cpoint = c;
}

void CSprite::replaceArea(int n,RectArea* area) {
	if (n < areas.size()) {
		RectArea::iterator iter2 ;
		CRectangle* rect;
		while ( !areas[n]->empty() ) {
			iter2 = areas[n]->begin();
			rect = *iter2;
			areas[n]->erase( iter2);
			delete rect;
		}
		delete areas[n];
		areas[n]=area;
	}
}

void CSprite::replaceRect(int area,int n,CRectangle* rect) {
	if ((area < areas.size()) && (n < areas[area]->size())) {
		delete (*areas[area])[n];
		(*areas[area])[n]=rect;
	}
}


int CSprite::size() {
	return (areas.size());
}
