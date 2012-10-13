#ifndef FS_SPRITE_RES__
#define FS_SPRITE_RES__

#include "Sprite.h"
#include "CanvasResource.h"
#include "Exception.h"

namespace FlatScene {

class SpriteResource : public CanvasResource {
    typedef Sprite  Handler;

    std::string     _name;
    Point           _cpoint;
    Areas           _areas;
    SpriteOpaque    _opaque;

public:
    void                setName(std::string newName) {
        _name = std::move(newName);
    }
    const std::string&  getName() const {
        return _name;
    }
    const Areas&        getAllAreas () const {
        return _areas;
    }
    const RectArea&     getArea(IndexArea index) const {
        auto it = _areas.find(index);
        if (it == _areas.end())
            throw Exception(std::string("no area with the index '")
                           +std::to_string(index)+"' has been found",__LINE__);
        return it->second;
    }
    const Point&        getCenter() const {
        return _cpoint;
    }
    void                setCenter(Point c) {
        _cpoint = std::move(c);
    }
    IndexArea           addArea(RectArea area) {
        typedef std::numeric_limits<IndexArea> limit;
        for (IndexArea index = limit::min(); index < limit::max(); index++) {
            if (_areas.find(index) == _areas.end()) {
                _areas[index] = area;
                return index;
            }
        }
        throw Exception("no index available for new area",__LINE__);
    }
    void                setArea(IndexArea n,RectArea area) {
        auto it = _areas.find(n);
        if (it != _areas.end())
            it->second = std::move(area);
        else
            throw Exception("::setArea, index not found");
    }
    int                 size() const {
        return _areas.size();
    }
    SpriteOpaque        isOpaque() {
        if (_opaque != SPRITE_OPAQUE_NOT_CHEQUED)
            return _opaque;

        _opaque = SPRITE_OPAQUE;

        try {
            for (int x = 0; x < getW() && _opaque == SPRITE_OPAQUE; x++ )
                for (int y = 0; y < getH() && _opaque == SPRITE_OPAQUE; y++ )
                    if ((getPixel(x,y) & 0xFF000000) != 0xFF000000)
                        _opaque = SPRITE_TRANSPARENT;
        } catch(Exception&) {
            throw Exception("This sprite doesn't contain accessible pixels for looking up the opacity");
        }

        return _opaque;
    }

    SpriteResource(ImageId id, BitmapGPU gpu) : CanvasResource(std::move(id), std::move(gpu)) {}
    virtual ~SpriteResource() {}

    friend class Sprite;
    friend class RefCountMemoryPolicy<SpriteResource>;
    friend class CanvasResource;
};

} // flatscene
#endif
