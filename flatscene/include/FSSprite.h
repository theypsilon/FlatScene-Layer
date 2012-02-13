#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "FSCanvas.h"
#include <string>
#include <vector>
using namespace std;

typedef vector<FSRectangle*> RectArea;

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
    string name;
    //Lista de Control points
    FSPoint* cpoint;
    vector<RectArea*> areas;

    //lo crea de una imagen fuente, a�adi�ndole el punto de control 0 si existe.
    FSSprite ( SCanvas pSurface, FSPoint* zerocpSource = NULL) ;
    //destruye el sprite.
    virtual ~FSSprite ( ) ;

    //devuelve puntero a la Imagen
    inline FSCanvas* getImage ( ) ;

    friend class FSSpriteset;
    friend class FSScreen;

    SpriteOpaque opaque;

public:
    //asigna nombre
    void setName(const char* newName);
    //devuelve nombre
    string getName();
    //devuelve la lista de puntos de control.
    vector<RectArea*>& getAllAreas () ;
    //devuelve el en�simo punto de control dentro del array de puntos de control.
    RectArea* getArea(unsigned int n);

    FSRectangle* getRect(unsigned int n,unsigned int m);

    FSPoint* getCenter();

    void replaceCenter(FSPoint* c);
    //a�ade al final de la lista el punto cpSource
    int addArea(RectArea* area);

    int addRect(unsigned int area,FSRectangle* rect);
    //substituye en el punto de control de la posici�n n, por cpSource
    void replaceArea(unsigned int n,RectArea* area);
    //devuelve el n�mero de Control Points que posee el Sprite.
    void replaceRect(unsigned int area,unsigned int n,FSRectangle* rect);

    int size();

    SpriteOpaque isOpaque();

    //renderiza el gr�fico.
    void put ( FSPoint& ptDst , Uint8 flags=0) ;

};

#endif
