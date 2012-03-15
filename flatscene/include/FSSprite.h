#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "FSCanvas.h"
#include <string>
#include <vector>
#include <memory>

namespace flatscene {

    enum SpriteOpaque {
        SPRITE_OPAQUE_NOT_CHEQUED,
        SPRITE_OPAQUE,
        SPRITE_TRANSPARENT,
        SPRITE_OPAQUE_TRANSFORMED
    };

    //CSprite--abstracts Image+Control Point.
    class FSSprite : public FSCanvas {
        typedef std::vector<FSRectangle> RectArea;
        typedef int IndexArea;
        typedef std::map<IndexArea,RectArea> Areas;
    private:

        FSSprite ( const FSSprite& ); //undefined
        //Nombre
        std::string name;
        //Lista de Control points
        FSPoint cpoint;
        Areas areas;

        friend class FSSpriteset;
        friend class FSScreen;

        SpriteOpaque opaque;

    public:
        //lo crea de una imagen fuente, a�adi�ndole el punto de control 0 si existe.

        FSSprite ();
        FSSprite ( FSCanvas&& pSurface, FSPoint zerocpSource = FSPoint(0,0)) ;

        FSSprite (FSSprite&& sprite);
        ~FSSprite();
        //asigna nombre
        void setName(const std::string& newName);
        //devuelve nombre
        const std::string& getName() const;
        //devuelve la lista de puntos de control.
        const Areas& getAllAreas () const;
        //devuelve el en�simo punto de control dentro del array de puntos de control.
        const RectArea& getArea(IndexArea index) const;

        const FSPoint& getCenter() const;

        void replaceCenter(FSPoint c);
        //a�ade al final de la lista el punto cpSource
        IndexArea addArea(RectArea area);

        //substituye en el punto de control de la posici�n n, por cpSource
        void replaceArea(IndexArea n,RectArea area);
        //devuelve el n�mero de Control Points que posee el Sprite.

        int size() const;

        SpriteOpaque isOpaque();

        //renderiza el gr�fico.
        void put ( FSPoint ptDst , Uint8 flags=0) const;
    private:
        struct SpriteImpl;
        //const std::unique_ptr<SpriteImpl> _impl;
    };

} // flatscene

#endif
