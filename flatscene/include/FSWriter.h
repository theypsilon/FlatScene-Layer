#ifndef __WRITE_H__
#define __WRITE_H__

#include "SDL_ttf.h"
#include <map>
#include <stack>
#include <list>
#include "FSSprite.h"
#include "FSColor.h"
#include "FSPoint.h"
#include "FSdefinitions.h"
#include "FSSingleton.h"
#include "FSNoncopyable.h"

enum TypeText {
    TT_LINE,
    TT_BOX
};

enum TypeColorTBox {
    TCTB_ALL,
    TCTB_TEXT,
    TCTB_BOX
};

class FSWriter : private FSNoncopyable, public FSSingleton<FSWriter> {
    friend class FSSingleton<FSWriter>;
public:
    int setfontSize(int newSize);

    int searchFont(const char* name,int withSize);
    int searchFont(const char* name);
    int searchFont(TTF_Font* fnt);
    int searchFont(int idtext);

    int loadFont(const char* fuente,int withSize);
    int loadFont(const char* fuente);

    int unloadFont(const char* fuente,int withSize);
    int unloadFont(const char* fuente);
    int unloadFont(int fuente);


    int line(int fuente,int x,int y,const char* text,...);
    int inBox(const char* file, int index);

    int erase(int text=ALL_TEXT,bool nextframe=false);


    int locateRenderScene ( float posx=0.0, float posy=0.0, float width=0.0, float height=0.0, float zoom = 1.0) ;

    int color(int text,float red, float green, float blue, float alpha, TypeColorTBox boxflags=TCTB_BOX, bool persistent=false);
    int color(int text,FSColor* col, float alpha,TypeColorTBox boxflags=TCTB_BOX, bool persistent=false);


    int render();

    void clear();
private:
    FSWriter();
    ~FSWriter();

    struct WriterImpl;
    WriterImpl* _impl;
    
    friend class FSScreen;
};


extern FSWriter& Write;

#endif
