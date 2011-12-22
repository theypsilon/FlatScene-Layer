#include "FSSprite.h"
#include "FSScreen.h"

FSSprite::FSSprite ( SCanvas pSurface, FSPoint* zerocpSource) : FSImage(pSurface), opaque(SPRITE_OPAQUE_NOT_CHEQUED) {

	if (zerocpSource==NULL) 
		cpoint =new FSPoint(0,0);
	else
		cpoint = zerocpSource;

}

FSSprite::~FSSprite ( ) {

	vector<RectArea*>::iterator iter ;
	RectArea* area ;
	while ( !areas.empty ( ) ) {
		iter = areas.begin ( ) ;
		area = *iter ;
		RectArea::iterator iter2 ;
		FSRectangle* rect;
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

FSImage* FSSprite::getImage ( ) {
	return ( (FSImage*) this ) ;
}

void FSSprite::put (FSPoint& ptDst ,Uint8 flags) {
	if (flags & 0x001) {
		ptDst.Y() -= cpoint->Y();
		ptDst.X() -= getImage()->getWidth() - cpoint->X();
	} else {
		ptDst -= *cpoint;
	}

	FSImage::put(ptDst,flags);
}
//TODO

void FSSprite::setName (const char *newName) {
	name=newName;
}

string FSSprite::getName() {
	return name;
}

int FSSprite::addRect(int area,FSRectangle* rect) {
	areas[area]->push_back(rect);
	return (areas[area]->size()-1);
}

int FSSprite::addArea(RectArea* area) {
	areas.push_back(area);
	return (areas.size()-1);
}


RectArea* FSSprite::getArea (int n ) 
{
#ifdef VECTOR_COMP
	if ((n<0) || (n >= areas.size()))
		return NULL;
#endif
	return (areas[n]) ;
}

FSRectangle* FSSprite::getRect (int n ,int m) 
{
#ifdef VECTOR_COMP
	if ((n<0) || (n >= areas.size()) || (m<0) || (m >= areas[n]->size()))
		return NULL;
#endif
	return ((*areas[n])[m]) ;
}

FSPoint* FSSprite::getCenter() {
	return cpoint;
}

void FSSprite::replaceCenter(FSPoint *c) {
	delete cpoint;
	cpoint = c;
}

void FSSprite::replaceArea(int n,RectArea* area) {
	if (n < areas.size()) {
		RectArea::iterator iter2 ;
		FSRectangle* rect;
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

void FSSprite::replaceRect(int area,int n,FSRectangle* rect) {
	if ((area < areas.size()) && (n < areas[area]->size())) {
		delete (*areas[area])[n];
		(*areas[area])[n]=rect;
	}
}


int FSSprite::size() {
	return (areas.size());
}

SpriteOpaque FSSprite::isOpaque() {
	if (opaque != SPRITE_OPAQUE_NOT_CHEQUED)
		return opaque;

	if (m_pSurface.sdl_surf == NULL)
		return opaque;

	opaque = SPRITE_OPAQUE;

	for (int x = 0; x < m_pSurface.sdl_surf->w && opaque == SPRITE_OPAQUE; x++ )
		for (int y = 0; y < m_pSurface.sdl_surf->h && opaque == SPRITE_OPAQUE; y++ ) {
			Uint32 pixel = getPixel(x,y);
			if ((pixel & 0xFF000000) != 0xFF000000)
				opaque = SPRITE_TRANSPARENT;
		}

	return opaque;
}