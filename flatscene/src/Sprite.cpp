#include "SpriteResource.h"
#include "Screen.h"
#include "Exception.h"
#include <limits>
#include <sstream>

namespace FlatScene {

    inline SpriteResource& _getSR(std::shared_ptr<CanvasResource>& p) {
        return static_cast<SpriteResource&>(*p);
    }

    inline const SpriteResource& _getSR(const std::shared_ptr<CanvasResource>& p) {
        return static_cast<const SpriteResource&>(*p);
    }

    Sprite::Sprite(std::shared_ptr<SpriteResource> res) 
        : Canvas(std::static_pointer_cast<CanvasResource>(res)) {}
    
    void Sprite::put (Point ptDst, unsigned char flags) const {
        auto& sr = _getSR(_res);
        if (flags & 0x001) {
            ptDst.y -= sr.getCenter().y;
            ptDst.x -= getWidth() - sr.getCenter().x;
        } else {
            ptDst.x -= sr.getCenter().x;
            ptDst.y -= sr.getCenter().y;
        }

        Canvas::put(ptDst,flags);
    }

    void Sprite::setName (std::string newName) {
        _getSR(_res).setName(std::move(newName));
    }

    const std::string& Sprite::getName() const {
        return _getSR(_res).getName();
    }

    IndexArea Sprite::addArea(RectArea area) {
        return _getSR(_res).addArea(area);
    }

    const RectArea& Sprite::getArea (IndexArea index) const {
        return _getSR(_res).getArea(index);
    }

    const Point& Sprite::getCenter() const {
        return _getSR(_res).getCenter();
    }

    void Sprite::setCenter(Point c) {
        _getSR(_res).setCenter(c);
    }

    void Sprite::setArea(IndexArea index,RectArea area) {
        _getSR(_res).setArea(index,area);
    }

    int Sprite::size() const {
        return _getSR(_res).size();
    }

    SpriteOpaque Sprite::isOpaque() {
        return _getSR(_res).isOpaque();
    }


    SoftwareSprite::SoftwareSprite(std::shared_ptr<SpriteResource> res) 
        : SoftwareCanvas(std::static_pointer_cast <CanvasResource>(res)) {}

    void SoftwareSprite::setName (std::string newName) {
        _getSR(_res).setName(std::move(newName));
    }

    const std::string& SoftwareSprite::getName() const {
        return _getSR(_res).getName();
    }

    IndexArea SoftwareSprite::addArea(RectArea area) {
        return _getSR(_res).addArea(area);
    }

    const RectArea& SoftwareSprite::getArea (IndexArea index) const {
        return _getSR(_res).getArea(index);
    }

    const Point& SoftwareSprite::getCenter() const {
        return _getSR(_res).getCenter();
    }

    void SoftwareSprite::setCenter(Point c) {
        _getSR(_res).setCenter(c);
    }

    void SoftwareSprite::setArea(IndexArea index,RectArea area) {
        _getSR(_res).setArea(index,area);
    }

    int SoftwareSprite::size() const {
        return _getSR(_res).size();
    }

    SpriteOpaque SoftwareSprite::isOpaque() {
        return _getSR(_res).isOpaque();
    }

} // flatscene
