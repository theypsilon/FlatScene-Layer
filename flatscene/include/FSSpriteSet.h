#ifndef __SPRITESET_H__
#define __SPRITESET_H__

#include "FSSprite.h"
#include "FSparserXML.h"
#include <vector>

typedef std::vector<FSSprite*> SpriteCollection;

typedef struct { 
    std::string name; 
    Uint8 mode; 
    int times;
    int number;
} SpritesetInfo ;

class FSSpriteset {
private :

    static Uint16 globalAuxiliar;
    //Inicializa Spriteset con los contenidos del fichero de tipo gr�fico, cuyo nombre es obtenido por el constructor.
    void loadChipset(std::string& c,Uint8 mode=ONLY_TEXTURE,std::string* cPrev=NULL);

    void loadChipsetSplit(std::string grd,Uint8 mode=ONLY_TEXTURE);

    std::string name;
    //Contenedor de Sprites.
    SpriteCollection m_vecSprites ;

    Uint8 mode;

    friend class FSImages;
    //construct empty sprite set
    FSSpriteset();
    //constructor que inicializa la colecci�n con un fichero externo.
    FSSpriteset(std::string c,Uint8 mode=ONLY_TEXTURE);

    virtual ~FSSpriteset();

    void add ( FSSprite* pspt ) ;
    bool setName(std::string& name);
    void remove ( FSSprite* pspt ) ;

    friend class FSScreen;

public:

    bool has ( FSSprite* pspt ) ;

    int search ( FSSprite* pspt ) ;

    FSSprite* get ( unsigned int n ) const;

    int size () ;

    std::string getName();

    Uint8 getMode();

};

typedef struct {
    Sint16 x, y,w, h;
} SDL_Rect_Signed;

typedef struct { 
    bool relative ; 
    std::vector<SDL_Rect_Signed> v; 
} RectsInfo;

#endif
