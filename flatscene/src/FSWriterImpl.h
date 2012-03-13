#ifndef __WRITE_IMPL_H__
#define __WRITE_IMPL_H__

#include "FSWriter.h"
#include "FSException.h"
#include <memory>

class FSEngine;

struct FSWriter::WriterImpl {
    struct SData;
    struct SFont;
    struct SEffectText;

    std::map<FSEngine*,SData> session;

    SData* data;
    FSEngine* admin;


    std::map<int,SFont> Fonts;
    std::map<SFont*,int> countFonts;
    std::list<int> lastIndexFontAdded;
    int fontSize;

    Float posx;
    Float posy;
    Float width;
    Float height;
    Float zoom;

    FSEngine* setAdmin(const FSEngine *const constAdmin);

    struct SChar {
        Uint16 glyph;
        FSFloatPoint* p;
    };

    struct SFont {
        const std::string cadena;
        TTF_Font * fuente;
        std::map<Uint16,FSCanvas> render;
        const unsigned int size;

        SFont(std::string cad, unsigned int sz) 
            : fuente(TTF_OpenFont((cad+".ttf").c_str(),sz)) 
            , cadena(cad)
            , size(sz)
            , render()
        {
            if (fuente == nullptr) throw FSException("No se ha cargado la fuente: "+cad+".ttf",__LINE__);
        }

        SFont(SFont&& fnt)
            : fuente(fnt.fuente)
            , cadena(std::move(fnt.cadena))
            , size(fnt.size)
            , render(std::move(fnt.render))
        {
            fnt.fuente = nullptr;
        }

        ~SFont() {
            if (fuente) TTF_CloseFont(fuente);
        }
    };

    struct SLineText{
        std::list<SChar> letra;
        SFont* fuente;

        ~SLineText();
    };

    struct FSTextBox {
        SFont* fuente;
        std::string file;
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
        Float red,green,blue,alpha;
        TypeColorTBox boxflags;
        bool persistent;
    };

    class FSText {
    private:
        TypeText type;
        FSText(const FSText&);
    public:
        union {
            SLineText* Line;
            FSTextBox* Box;
        };

        SEffectText* fx;

        FSText(const char* file,const char* text,int x,int y,int Lim,SFont* ttf_fnt,int next);
        FSText();
        FSText(FSText&& mv);
        ~FSText();
        TypeText Type() const;
    };

    struct SData {
        std::map<int,FSText*> Texts;

        std::list<int> lastIndexTextAdded;

        std::list<int> deleteTextBuffer;

        SDL_Color fgcolor;

        SData() { fgcolor.r = fgcolor.b = fgcolor.g = 255; }
    };
};

#endif
