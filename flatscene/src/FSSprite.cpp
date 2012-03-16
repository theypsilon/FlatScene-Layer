#include "FSSpriteImpl.h"
#include "FSScreen.h"
#include "FSException.h"
#include <limits>
#include <sstream>

namespace flatscene {

Sprite::Sprite() {}

Sprite::Sprite ( Canvas&& pSurface, Point zerocpSource) 
    : Canvas(std::move(pSurface)), opaque(SPRITE_OPAQUE_NOT_CHEQUED), cpoint(zerocpSource)
{}

Sprite::Sprite(Sprite&& spt) 
    : Canvas(std::move(spt))
    , areas(std::move(spt.areas))
    , name(std::move(spt.name))
    , cpoint(std::move(spt.cpoint))
    , opaque(spt.opaque) 
{}

Sprite::~Sprite() {}

void Sprite::put (Point ptDst ,Uint8 flags) const {
    if (flags & 0x001) {
        ptDst.y -= cpoint.y;
        ptDst.x -= getWidth() - cpoint.x;
    } else {
        ptDst.x -= cpoint.x;
        ptDst.y -= cpoint.y;
    }

    Canvas::put(ptDst,flags);
}
//TODO

void Sprite::setName (const std::string& newName) {
    name=newName;
}

const std::string& Sprite::getName() const {
    return name;
}

Sprite::IndexArea Sprite::addArea(RectArea area) {
    for (IndexArea index = std::numeric_limits<IndexArea>::min(); index < std::numeric_limits<IndexArea>::max(); index++) {
        if (areas.find(index) == areas.end()) {
            areas[index] = area;
            return index;
        }
    }
    throw Exception("no index available for new area",__LINE__);
}


const Sprite::RectArea& Sprite::getArea (IndexArea index) const {
    std::remove_const<decltype(areas.find(index))>::type it = areas.find(index);
    if (it != areas.end())
        return it->second;
    std::stringstream ss;
    ss << "no area with the index '" << index << "' has been found";
    throw Exception(ss.str(),__LINE__);
}

const Point& Sprite::getCenter() const {
    return cpoint;
}

void Sprite::replaceCenter(Point c) {
    cpoint = c;
}

void Sprite::replaceArea(IndexArea index,RectArea area) {
    auto old = const_cast<std::remove_const<decltype(this->getArea(index))>::type>(this->getArea(index));
    std::swap(old,area);
}

int Sprite::size() const {
    return areas.size();
}

SpriteOpaque Sprite::isOpaque() {
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

} // flatscene
