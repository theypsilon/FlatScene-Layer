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
            ptDst.y -= getRes<SpriteResource>().getCenter().y;
            ptDst.x -= getWidth() - getRes<SpriteResource>().getCenter().x;
        } else {
            ptDst.x -= getRes<SpriteResource>().getCenter().x;
            ptDst.y -= getRes<SpriteResource>().getCenter().y;
        }

        Canvas::put(ptDst,flags);
    }

    void Sprite::setName (std::string newName) {
        getRes<SpriteResource>().setName(std::move(newName));
    }

    const std::string& Sprite::getName() const {
        return getRes<SpriteResource>().getName();
    }

    IndexArea Sprite::addArea(RectArea area) {
        return getRes<SpriteResource>().addArea(area);
    }

    const RectArea& Sprite::getArea (IndexArea index) const {
        return getRes<SpriteResource>().getArea(index);
    }

    const Point& Sprite::getCenter() const {
        return getRes<SpriteResource>().getCenter();
    }

    void Sprite::setCenter(Point c) {
        getRes<SpriteResource>().setCenter(c);
    }

    void Sprite::setArea(IndexArea index,RectArea area) {
        getRes<SpriteResource>().setArea(index,area);
    }

    int Sprite::size() const {
        return getRes<SpriteResource>().size();
    }

    SpriteOpaque Sprite::isOpaque() {
        return getRes<SpriteResource>().isOpaque();
    }


    SoftwareSprite::SoftwareSprite(SpriteResource* res) 
        : SoftwareCanvas(res)
    {}

    void SoftwareSprite::setName (std::string newName) {
        getRes<SpriteResource>().setName(std::move(newName));
    }

    const std::string& SoftwareSprite::getName() const {
        return getRes<SpriteResource>().getName();
    }

    IndexArea SoftwareSprite::addArea(RectArea area) {
        return getRes<SpriteResource>().addArea(area);
    }

    const RectArea& SoftwareSprite::getArea (IndexArea index) const {
        return getRes<SpriteResource>().getArea(index);
    }

    const Point& SoftwareSprite::getCenter() const {
        return getRes<SpriteResource>().getCenter();
    }

    void SoftwareSprite::setCenter(Point c) {
        getRes<SpriteResource>().setCenter(c);
    }

    void SoftwareSprite::setArea(IndexArea index,RectArea area) {
        getRes<SpriteResource>().setArea(index,area);
    }

    int SoftwareSprite::size() const {
        return getRes<SpriteResource>().size();
    }

    SpriteOpaque SoftwareSprite::isOpaque() {
        return getRes<SpriteResource>().isOpaque();
    }

} // flatscene
