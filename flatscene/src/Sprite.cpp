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
            ptDst.y -= getResy<SpriteResource>().getCenter().y;
            ptDst.x -= getWidth() - getResy<SpriteResource>().getCenter().x;
        } else {
            ptDst.x -= getResy<SpriteResource>().getCenter().x;
            ptDst.y -= getResy<SpriteResource>().getCenter().y;
        }

        Canvas::put(ptDst,flags);
    }

    void Sprite::setName (std::string newName) {
        getResy<SpriteResource>().setName(std::move(newName));
    }

    const std::string& Sprite::getName() const {
        return getResy<SpriteResource>().getName();
    }

    IndexArea Sprite::addArea(RectArea area) {
        return getResy<SpriteResource>().addArea(area);
    }

    const RectArea& Sprite::getArea (IndexArea index) const {
        return getResy<SpriteResource>().getArea(index);
    }

    const Point& Sprite::getCenter() const {
        return getResy<SpriteResource>().getCenter();
    }

    void Sprite::setCenter(Point c) {
        getResy<SpriteResource>().setCenter(c);
    }

    void Sprite::setArea(IndexArea index,RectArea area) {
        getResy<SpriteResource>().setArea(index,area);
    }

    int Sprite::size() const {
        return getResy<SpriteResource>().size();
    }

    SpriteOpaque Sprite::isOpaque() {
        return getResy<SpriteResource>().isOpaque();
    }


    SoftwareSprite::SoftwareSprite(SpriteResource* res) 
        : SoftwareCanvas(res)
    {}

    void SoftwareSprite::setName (std::string newName) {
        getResy<SpriteResource>().setName(std::move(newName));
    }

    const std::string& SoftwareSprite::getName() const {
        return getResy<SpriteResource>().getName();
    }

    IndexArea SoftwareSprite::addArea(RectArea area) {
        return getResy<SpriteResource>().addArea(area);
    }

    const RectArea& SoftwareSprite::getArea (IndexArea index) const {
        return getResy<SpriteResource>().getArea(index);
    }

    const Point& SoftwareSprite::getCenter() const {
        return getResy<SpriteResource>().getCenter();
    }

    void SoftwareSprite::setCenter(Point c) {
        getResy<SpriteResource>().setCenter(c);
    }

    void SoftwareSprite::setArea(IndexArea index,RectArea area) {
        getResy<SpriteResource>().setArea(index,area);
    }

    int SoftwareSprite::size() const {
        return getResy<SpriteResource>().size();
    }

    SpriteOpaque SoftwareSprite::isOpaque() {
        return getResy<SpriteResource>().isOpaque();
    }

} // flatscene
