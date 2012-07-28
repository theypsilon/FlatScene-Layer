#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "Canvas.h"
#include <string>
#include <vector>
#include <memory>

namespace FlatScene {

    enum SpriteOpaque {
        SPRITE_OPAQUE_NOT_CHEQUED,
        SPRITE_OPAQUE,
        SPRITE_TRANSPARENT,
        SPRITE_OPAQUE_TRANSFORMED
    };

    class SpriteResource;

    class Sprite : public Canvas {
    public:
        typedef std::vector<Rectangle> RectArea;
        typedef int IndexArea;
        typedef std::map<IndexArea,RectArea> Areas;

        Sprite ();

        void                setName(const std::string& newName);
        const std::string&  getName() const;
        const Areas&        getAllAreas () const;
        const RectArea&     getArea(IndexArea index) const;
        const Point&        getCenter() const;
        void                setCenter(Point c);
        IndexArea           addArea(RectArea area);
        void                setArea(IndexArea n,RectArea area);
        int                 size() const;
        SpriteOpaque        isOpaque();

        void                put (Point ptDst, unsigned char flags=0) const;

    private:

        friend class Spriteset;
        friend class Screen;
    };

} // flatscene

#endif
