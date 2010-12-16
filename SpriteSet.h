#ifndef __SPRITESET_H__
#define __SPRITESET_H__

#include "Sprite.h"
#include "parserXML.h"
#include <vector>

typedef vector<CSprite*> SpriteCollection;

typedef struct { 
	string name; 
	Uint8 mode; 
	int times;
	int number;
} SpritesetInfo ;

class CSpriteset {
private :

	static Uint16 globalAuxiliar;
	//Inicializa Spriteset con los contenidos del fichero de tipo gráfico, cuyo nombre es obtenido por el constructor.
	void loadChipset(string& c,Uint8 mode=ONLY_TEXTURE,string* cPrev=NULL);

	string name;
	//Contenedor de Sprites.
	SpriteCollection m_vecSprites ;

	Uint8 mode;

	friend class CControlImages;
	//construct empty sprite set
	CSpriteset();
	//constructor que inicializa la colección con un fichero externo.
	CSpriteset(string c,Uint8 mode=ONLY_TEXTURE);

	virtual ~CSpriteset();

	void add ( CSprite* pspt ) ;
	bool setName(string& name);
	void remove ( CSprite* pspt ) ;
	SpriteCollection& getSpriteList ( ) ;

	friend class CScreen;

public:

	bool has ( CSprite* pspt ) ;

	int search ( CSprite* pspt ) ;

	CSprite* get ( int n ) ;

	int size () ;

	string getName();

	Uint8 getMode();

};

typedef struct {
	Sint16 x, y,w, h;
} SDL_Rect_Signed;

typedef struct { 
	bool relative ; 
	vector<SDL_Rect_Signed> v; 
} RectsInfo;

#endif
