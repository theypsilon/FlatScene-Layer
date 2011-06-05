#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "FSImage.h"
#include <string>
#include <vector>
using namespace std;

typedef vector<CRectangle*> RectArea;

enum SpriteOpaque {
	SPRITE_OPAQUE_NOT_CHEQUED,
	SPRITE_OPAQUE,
	SPRITE_TRANSPARENT,
	SPRITE_OPAQUE_TRANSFORMED
};

//CSprite--abstracts Image+Control Point.
class CSprite : public CImage
{
private:
	//Nombre
	string name;
	//Lista de Control points
	CPoint* cpoint;
	vector<RectArea*> areas;

	//lo crea de una imagen fuente, a�adi�ndole el punto de control 0 si existe.
	CSprite ( SCanvas pSurface, CPoint* zerocpSource = NULL) ;
	//destruye el sprite.
	virtual ~CSprite ( ) ;

	//devuelve puntero a la Imagen
	inline CImage* getImage ( ) ;

	friend class CSpriteset;
	friend class CScreen;

	SpriteOpaque opaque;

public:
	//asigna nombre
	void setName(const char* newName);
	//devuelve nombre
	string getName();
	//devuelve la lista de puntos de control.
	vector<RectArea*>& getAllAreas () ;
	//devuelve el en�simo punto de control dentro del array de puntos de control.
	RectArea* getArea(int n);

	CRectangle* getRect(int n,int m);

	CPoint* getCenter();

	void replaceCenter(CPoint* c);
	//a�ade al final de la lista el punto cpSource
	int addArea(RectArea* area);

	int addRect(int area,CRectangle* rect);
	//substituye en el punto de control de la posici�n n, por cpSource
	void replaceArea(int n,RectArea* area);
	//devuelve el n�mero de Control Points que posee el Sprite.
	void replaceRect(int area,int n,CRectangle* rect);

	int size();

	SpriteOpaque isOpaque();

	//renderiza el gr�fico.
	void put ( CPoint& ptDst , Uint8 flags=0) ;

};

#endif
