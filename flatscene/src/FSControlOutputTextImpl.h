#ifndef __WRITE_IMPL_H__
#define __WRITE_IMPL_H__

#include "FSControlOutputText.h"

class FSEngine;

struct FSControlOutputText::FSControlOutputTextImpl {
    struct SData;
    struct SFont;
    struct SEffectText;

    map<FSEngine*,SData*> session;

    SData* data;
    FSEngine* admin;


    map<int,SFont*> Fonts;
    map<SFont*,int> countFonts;
    list<int> lastIndexFontAdded;
    int fontSize;

    float posx;
    float posy;
    float width;
    float height;
    float zoom;

    static FSControlOutputText* singleton;

    FSEngine* setAdmin(FSEngine* newAdmin);

    struct SChar {
        Uint16 glyph;
        FSFloatPoint* p;
    };

    struct SFont {
        string cadena;
        TTF_Font* fuente;
        map<Uint16,FSImage*> render;
        int size;
    };

    struct SLineText{
        list<SChar> letra;
        SFont* fuente;

        ~SLineText();
    };

    struct FSTextBox {
        SFont* fuente;
        string file;
        int next;

        list<SChar> charInDisplay;
        FSPoint upleft;

        SEffectText* fx;

        int timer;
        SDL_Color col;

        FSImage* box;
        Uint8 xBox;
        Uint8 yBox;

        int step;
        int maxStep;

        FSTextBox(const char* file,const char* text,int x,int y,int Lim,SFont* ttf_fnt,int next);

        virtual ~FSTextBox();

        virtual int update();

        virtual void deleteBox();

        virtual void createBox();

        virtual int finish();
    };

    struct SEffectText {
        GLfloat red,green,blue,alpha;
        TypeColorTBox boxflags;
        bool persistent;
    };

    class FSText {
    private:
        TypeText type;
    public:
        union {
            SLineText* Line;
            FSTextBox* Box;
        };

        SEffectText* fx;

        FSText(const char* file,const char* text,int x,int y,int Lim,SFont* ttf_fnt,int next);
        FSText();
        ~FSText();
        TypeText Type() const;
    };

    struct SData {
        map<int,FSText*> Texts;

        list<int> lastIndexTextAdded;

        list<int> deleteTextBuffer;

        SDL_Color fgcolor;
    };
};

#endif
