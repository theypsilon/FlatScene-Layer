#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "FSCanvas.h"
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

    //CSprite--abstracts Image+Control Point.
    class Sprite : public Canvas {
        typedef std::vector<Rectangle> RectArea;
        typedef int IndexArea;
        typedef std::map<IndexArea,RectArea> Areas;
    private:

        Sprite ( const Sprite& ); //undefined
        //Nombre
        std::string name;
        //Lista de Control points
        Point cpoint;
        Areas areas;

        friend class Spriteset;
        friend class Screen;

        SpriteOpaque opaque;

    public:
        //lo crea de una imagen fuente, a�adi�ndole el punto de control 0 si existe.

        Sprite ();
        Sprite ( Canvas&& pSurface, Point zerocpSource = Point(0,0)) ;

        Sprite (Sprite&& sprite);
        ~Sprite();
        //asigna nombre
        void setName(const std::string& newName);
        //devuelve nombre
        const std::string& getName() const;
        //devuelve la lista de puntos de control.
        const Areas& getAllAreas () const;
        //devuelve el en�simo punto de control dentro del array de puntos de control.
        const RectArea& getArea(IndexArea index) const;

        const Point& getCenter() const;

        void replaceCenter(Point c);
        //a�ade al final de la lista el punto cpSource
        IndexArea addArea(RectArea area);

        //substituye en el punto de control de la posici�n n, por cpSource
        void replaceArea(IndexArea n,RectArea area);
        //devuelve el n�mero de Control Points que posee el Sprite.

        int size() const;

        SpriteOpaque isOpaque();

        //renderiza el gr�fico.
        void put ( Point ptDst , Uint8 flags=0) const;
    private:
        struct SpriteImpl;
        std::unique_ptr<SpriteImpl> _impl;
    };

} // flatscene

#endif
