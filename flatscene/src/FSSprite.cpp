#include "FSSprite.h"
#include "FSScreen.h"
#include "FSException.h"
#include <limits>
#include <sstream>

FSSprite::FSSprite ( const SCanvas& pSurface, FSPoint zerocpSource) 
: FSCanvas(pSurface), opaque(SPRITE_OPAQUE_NOT_CHEQUED), cpoint(zerocpSource) {}

FSSprite::FSSprite ( FSCanvas pSurface, FSPoint zerocpSource) 
: FSCanvas(std::move(pSurface)), opaque(SPRITE_OPAQUE_NOT_CHEQUED), cpoint(zerocpSource) {}

FSSprite::FSSprite(FSSprite&& spt) : FSCanvas(std::move(spt)) {
    areas = std::move(spt.areas);
    name = std::move(spt.name);
    cpoint = std::move(spt.cpoint);
    opaque = spt.opaque;
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

FSSprite::IndexArea FSSprite::addArea(RectArea area) {
    for (IndexArea index = std::numeric_limits<IndexArea>::min(); index < std::numeric_limits<IndexArea>::max(); index++) {
        if (areas.find(index) == areas.end()) {
            areas[index] = area;
            return index;
        }
    }
    throw FSException("no index available for new area",__LINE__);
}


const FSSprite::RectArea& FSSprite::getArea (IndexArea index) const {
    std::remove_const<decltype(areas.find(index))>::type it = areas.find(index);
    if (it != areas.end())
        return it->second;
    std::stringstream ss;
    ss << "no area with the index '" << index << "' has been found";
    throw FSException(ss.str(),__LINE__);
}

const FSPoint& FSSprite::getCenter() const {
    return cpoint;
}

void FSSprite::replaceCenter(FSPoint c) {
    cpoint = c;
}

void FSSprite::replaceArea(IndexArea index,RectArea area) {
    auto old = const_cast<std::remove_const<decltype(this->getArea(index))>::type>(this->getArea(index));
    std::swap(old,area);
}

int FSSprite::size() const {
    return areas.size();
}

SpriteOpaque FSSprite::isOpaque() {
    if (opaque != SPRITE_OPAQUE_NOT_CHEQUED)
        return opaque;

    if (sdl_surf == NULL)
        return opaque;

    opaque = SPRITE_OPAQUE;

    for (int x = 0; x < sdl_surf->w && opaque == SPRITE_OPAQUE; x++ )
        for (int y = 0; y < sdl_surf->h && opaque == SPRITE_OPAQUE; y++ ) {
            Uint32 pixel = getPixel(x,y);
            if ((pixel & 0xFF000000) != 0xFF000000)
                opaque = SPRITE_TRANSPARENT;
        }

    return opaque;
}
