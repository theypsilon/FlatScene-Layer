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

    //CSprite--abstracts Image+Control Point.
    class Sprite : public Canvas {
    public:
        typedef std::vector<Rectangle> RectArea;
        typedef int IndexArea;
        typedef std::map<IndexArea,RectArea> Areas;
        //lo crea de una imagen fuente, a�adi�ndole el punto de control 0 si existe.

        Sprite ();
        Sprite ( Canvas&& pSurface, Point zerocpSource = Point(0,0)) ;

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

        template <typename Resource = SpriteResource> inline Resource& getRes() const {
            return Canvas::getRes<Resource>();
        }
    };

} // flatscene

#endif
