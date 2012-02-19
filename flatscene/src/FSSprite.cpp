#include "FSSprite.h"
#include "FSScreen.h"

FSSprite::FSSprite ( SCanvas pSurface, FSPoint zerocpSource) 
: FSCanvas(pSurface), opaque(SPRITE_OPAQUE_NOT_CHEQUED), cpoint(zerocpSource) {}

FSSprite::~FSSprite ( ) {

    if (m_pSurface.sdl_surf) {
        SDL_FreeSurface(m_pSurface.sdl_surf);
    }

    if (m_pSurface.h != 0 || m_pSurface.w !=0)
        glDeleteTextures( 1, &(m_pSurface.tex) );

    clearSurface();
    
}

void FSSprite::put (FSPoint ptDst ,Uint8 flags) const {
    if (flags & 0x001) {
        ptDst.y -= cpoint.y;
        ptDst.x -= getWidth() - cpoint.x;
    } else {
        ptDst.x -= cpoint.x;
        ptDst.y -= cpoint.y;
    }

    FSCanvas::put(ptDst,flags);
}
//TODO

void FSSprite::setName (const std::string& newName) {
    name=newName;
}

const std::string& FSSprite::getName() const {
    return name;
}

int FSSprite::addRect(unsigned int area,FSRectangle rect) {
    areas.at(area).push_back(rect);
    return (areas.at(area).size()-1);
}

int FSSprite::addArea(RectArea area) {
    areas.push_back(area);
    return (areas.size()-1);
}


const RectArea& FSSprite::getArea (unsigned int n ) const {
#ifdef VECTOR_COMP
    if ((n<0) || (n >= areas.size()))
        return NULL;
#endif
    return (areas[n]) ;
}

const FSRectangle& FSSprite::getRect (unsigned int n ,unsigned int m) const {
#ifdef VECTOR_COMP
    if ((n<0) || (n >= areas.size()) || (m<0) || (m >= areas[n]->size()))
        return NULL;
#endif
    return areas[n][m];
}

const FSPoint& FSSprite::getCenter() const {
    return cpoint;
}

void FSSprite::replaceCenter(FSPoint c) {
    cpoint = c;
}

void FSSprite::replaceArea(unsigned int n,RectArea area) {
    if (n < areas.size()) {
        areas[n]=area;
    }
}

void FSSprite::replaceRect(unsigned int area,unsigned int n,FSRectangle rect) {
    if ((area < areas.size()) && (n < areas[area].size())) {
        areas[area][n]=rect;
    }
}


int FSSprite::size() const {
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
