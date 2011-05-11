#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "Image.h"
#include <string>
#include <vector>
using namespace std;

typedef vector<CRectangle*> RectArea;

//CSprite--abstracts Image+Control Point.
class CSprite : public CImage
{
private:
	//Nombre
	string name;
	//Lista de Control points
	CPoint* cpoint;
	vector<RectArea*> areas;

	//lo crea de una imagen fuente, añadiéndole el punto de control 0 si existe.
	CSprite ( SCanvas pSurface, CPoint* zerocpSource = NULL) ;
	//destruye el sprite.
	virtual ~CSprite ( ) ;

	//devuelve puntero a la Imagen
	inline CImage* getImage ( ) ;

	friend class CSpriteset;
	friend class CScreen;

public:
	//asigna nombre
	void setName(const char* newName);
	//devuelve nombre
	string getName();
	//devuelve la lista de puntos de control.
	vector<RectArea*>& getAllAreas () ;
	//devuelve el enésimo punto de control dentro del array de puntos de control.
	RectArea* getArea(int n);

	CRectangle* getRect(int n,int m);

	CPoint* getCenter();

	void replaceCenter(CPoint* c);
	//añade al final de la lista el punto cpSource
	int addArea(RectArea* area);

	int addRect(int area,CRectangle* rect);
	//substituye en el punto de control de la posición n, por cpSource
	void replaceArea(int n,RectArea* area);
	//devuelve el número de Control Points que posee el Sprite.
	void replaceRect(int area,int n,CRectangle* rect);

	int size();


	//renderiza el gráfico.
	void put ( CPoint& ptDst , Uint8 flags=0) ;

};

#endif
