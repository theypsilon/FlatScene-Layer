#ifndef __WRITE_IMPL_H__
#define __WRITE_IMPL_H__

#include "FSWriter.h"

class FSEngine;

struct FSWriter::WriterImpl {
    struct SData;
    struct SFont;
    struct SEffectText;

    std::map<FSEngine*,SData*> session;

    SData* data;
    FSEngine* admin;


    std::map<int,SFont*> Fonts;
    std::map<SFont*,int> countFonts;
    std::list<int> lastIndexFontAdded;
    int fontSize;

    float posx;
    float posy;
    float width;
    float height;
    float zoom;

    FSEngine* setAdmin(FSEngine* newAdmin);

    struct SChar {
        Uint16 glyph;
        FSFloatPoint* p;
    };

    struct SFont {
        string cadena;
        TTF_Font* fuente;
        std::map<Uint16,FSCanvas*> render;
        unsigned int size;
    };

    struct SLineText{
        std::list<SChar> letra;
        SFont* fuente;

        ~SLineText();
    };

    struct FSTextBox {
        SFont* fuente;
        string file;
        int next;

        std::list<SChar> charInDisplay;
        FSPoint upleft;

        SEffectText* fx;

        unsigned int timer;
        SDL_Color col;

        FSCanvas* box;
        Uint8 xBox;
        Uint8 yBox;

        unsigned int step;
        unsigned int maxStep;

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
        std::map<int,FSText*> Texts;

        std::list<int> lastIndexTextAdded;

        std::list<int> deleteTextBuffer;

        SDL_Color fgcolor;
    };
};

#endif
