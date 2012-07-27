#include "SpriteResource.h"
#include "Screen.h"
#include "Exception.h"
#include <limits>
#include <sstream>

namespace FlatScene {

Sprite::Sprite() 
    : Canvas(new SpriteResource)
{}

void Sprite::put (Point ptDst, unsigned char flags) const {
    if (flags & 0x001) {
        ptDst.y -= getRes<SpriteResource>().cpoint.y;
        ptDst.x -= getWidth() - getRes<SpriteResource>().cpoint.x;
    } else {
        ptDst.x -= getRes<SpriteResource>().cpoint.x;
        ptDst.y -= getRes<SpriteResource>().cpoint.y;
    }

    Canvas::put(ptDst,flags);
}
//TODO

void Sprite::setName (const std::string& newName) {
    getRes<SpriteResource>().name = newName;
}

const std::string& Sprite::getName() const {
    return getRes<SpriteResource>().name;
}

Sprite::IndexArea Sprite::addArea(RectArea area) {
    for (IndexArea index = std::numeric_limits<IndexArea>::min(); index < std::numeric_limits<IndexArea>::max(); index++) {
        if (getRes<SpriteResource>().areas.find(index) == getRes<SpriteResource>().areas.end()) {
            getRes<SpriteResource>().areas[index] = area;
            return index;
        }
    }
    throw Exception("no index available for new area",__LINE__);
}


const Sprite::RectArea& Sprite::getArea (IndexArea index) const {
    std::remove_const<decltype(getRes<SpriteResource>().areas.find(index))>::type it = getRes<SpriteResource>().areas.find(index);
    if (it != getRes<SpriteResource>().areas.end())
        return it->second;
    std::stringstream ss;
    ss << "no area with the index '" << index << "' has been found";
    throw Exception(ss.str(),__LINE__);
}

const Point& Sprite::getCenter() const {
    return getRes<SpriteResource>().cpoint;
}

void Sprite::setCenter(Point c) {
    getRes<SpriteResource>().cpoint = c;
}

void Sprite::setArea(IndexArea index,RectArea area) {
    auto old = const_cast<std::remove_const<decltype(this->getArea(index))>::type>(this->getArea(index));
    std::swap(old,area);
}

int Sprite::size() const {
    return getRes<SpriteResource>().areas.size();
}

SpriteOpaque Sprite::isOpaque() {
    /*if (opaque != SPRITE_OPAQUE_NOT_CHEQUED)
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
*/
    return getRes<SpriteResource>().opaque;
}

} // flatscene
