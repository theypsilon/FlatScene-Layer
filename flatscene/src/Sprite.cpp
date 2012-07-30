#include "SpriteResource.h"
#include "Screen.h"
#include "Exception.h"
#include <limits>
#include <sstream>

namespace FlatScene {

    Sprite::Sprite(SpriteResource* res) 
        : Canvas(res)
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

    void Sprite::setName (const std::string& newName) {
        getRes<SpriteResource>().name = newName;
    }

    const std::string& Sprite::getName() const {
        return getRes<SpriteResource>().name;
    }

    Sprite::IndexArea Sprite::addArea(RectArea area) {
        typedef std::numeric_limits<IndexArea> limit;
        for (IndexArea index = limit::min(); index < limit::max(); index++) {
            if (getRes<SpriteResource>().areas.find(index) == getRes<SpriteResource>().areas.end()) {
                getRes<SpriteResource>().areas[index] = area;
                return index;
            }
        }
        throw Exception("no index available for new area",__LINE__);
    }


    const Sprite::RectArea& Sprite::getArea (IndexArea index) const {
        auto it = getRes<SpriteResource>().areas.find(index);
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
        typedef std::remove_const<decltype(this->getArea(index))>::type AreaNoConst;
        auto old = const_cast<AreaNoConst>(this->getArea(index));
        std::swap(old,area);
    }

    int Sprite::size() const {
        return getRes<SpriteResource>().areas.size();
    }

    SpriteOpaque Sprite::isOpaque() {
        auto& opaque = getRes<SpriteResource>().opaque;
        if (opaque != SPRITE_OPAQUE_NOT_CHEQUED)
            return opaque;

        opaque = SPRITE_OPAQUE;

        try {
            for (int x = 0; x < getWidth() && opaque == SPRITE_OPAQUE; x++ )
                for (int y = 0; y < getHeight() && opaque == SPRITE_OPAQUE; y++ )
                    if ((getPixel(x,y) & 0xFF000000) != 0xFF000000)
                        opaque = SPRITE_TRANSPARENT;
        } catch(Exception&) {
            throw Exception("This sprite doesn't contain accessible pixels for looking up the opacity");
        }

        return opaque;
    }

} // flatscene
