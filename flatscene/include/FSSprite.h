#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "FSCanvas.h"
#include <string>
#include <vector>

typedef std::vector<FSRectangle> RectArea;

enum SpriteOpaque {
    SPRITE_OPAQUE_NOT_CHEQUED,
    SPRITE_OPAQUE,
    SPRITE_TRANSPARENT,
    SPRITE_OPAQUE_TRANSFORMED
};

//CSprite--abstracts Image+Control Point.
class FSSprite : public FSCanvas
{
private:
    //Nombre
    std::string name;
    //Lista de Control points
    FSPoint cpoint;
    std::vector<RectArea> areas;

    //lo crea de una imagen fuente, a�adi�ndole el punto de control 0 si existe.
    FSSprite ( SCanvas pSurface = SCanvas(), FSPoint zerocpSource = FSPoint(0,0)) ;

    FSSprite (FSSprite&& sprite);
    //destruye el sprite.
    virtual ~FSSprite ( ) ;

    friend class FSSpriteset;
    friend class FSScreen;

    SpriteOpaque opaque;

public:
    //asigna nombre
    void setName(const std::string& newName);
    //devuelve nombre
    const std::string& getName() const;
    //devuelve la lista de puntos de control.
    const std::vector<RectArea>& getAllAreas () const;
    //devuelve el en�simo punto de control dentro del array de puntos de control.
    const RectArea& getArea(unsigned int n) const;

    const FSRectangle& getRect(unsigned int n,unsigned int m) const;

    const FSPoint& getCenter() const;

    void replaceCenter(FSPoint c);
    //a�ade al final de la lista el punto cpSource
    int addArea(RectArea area);

    int addRect(unsigned int area,FSRectangle rect);
    //substituye en el punto de control de la posici�n n, por cpSource
    void replaceArea(unsigned int n,RectArea area);
    //devuelve el n�mero de Control Points que posee el Sprite.
    void replaceRect(unsigned int area,unsigned int n,FSRectangle rect);

    int size() const;

    SpriteOpaque isOpaque();

    //renderiza el gr�fico.
    void put ( FSPoint ptDst , Uint8 flags=0) const;

};

#endif
