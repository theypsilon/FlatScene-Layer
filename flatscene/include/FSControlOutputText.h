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
#include "FSTextBox.h"

class FSEngine;

enum TypeText {
    TT_LINE,
    TT_BOX
};

struct SEffectText {
    GLfloat red,green,blue,alpha;
    TypeColorTBox boxflags;
    bool persistent;
};

struct SText {
private:
    TypeText type;
public:
    union {
        SLineText* Line;
        FSTextBox* Box;
    };

    SEffectText* fx;

    SText(const char* file,const char* text,int x,int y,int Lim,SFont* ttf_fnt,int next) {

        fx = NULL;
        Box = new FSTextBox(file,text,x,y,Lim,ttf_fnt,next);
        type = TT_BOX;
    }

    SText() {
        fx = NULL;
        Line = new SLineText;
        type = TT_LINE;
    }

    ~SText() {
        if (fx) {
            delete fx;
            fx=NULL;
        }

        if (type == TT_BOX) {
            delete Box;
            Box = NULL;
        } else if (type == TT_LINE) {
            delete Line;
            Line = NULL;
        }

    }

    TypeText Type() {
        return type;
    }

};

struct SData {
    map<int,SText*> Texts;

    list<int> lastIndexTextAdded;

    list<int> deleteTextBuffer;

    SDL_Color fgcolor;
};

class FSControlOutputText {
public:
    FSControlOutputText();
    ~FSControlOutputText();

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
	friend class FSScreen;
    struct FSControlOutputTextImpl;
    FSControlOutputTextImpl* _impl;
};

extern FSControlOutputText Write;

#endif
