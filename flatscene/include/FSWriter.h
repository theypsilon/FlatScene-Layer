#ifndef __WRITE_H__
#define __WRITE_H__

#include <map>
#include <stack>
#include <list>
#include "FSSprite.h"
#include "FSColor.h"
#include "FSTypes.h"
#include "FSdefinitions.h"
#include "FSSingleton.h"
#include "FSNoncopyable.h"
#include "FSPimpl.h"

namespace flatscene {

    enum TypeText {
        TT_LINE,
        TT_BOX
    };

    enum TypeColorTBox {
        TCTB_ALL,
        TCTB_TEXT,
        TCTB_BOX
    };

    class Writer : private Noncopyable, public util::Singleton<Writer> {
        friend class util::Singleton<Writer>;
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
        void lineOnce(int fuente, int x,int y, const char* text,...);
        int inBox(const char* file, int index);

        int erase(int text=ALL_TEXT,bool nextframe=false);


        int locateRenderScene ( Float posx=0.0, Float posy=0.0, Float width=0.0, Float height=0.0, Float zoom = 1.0) ;

        int color(int text,Float red, Float green, Float blue, Float alpha, TypeColorTBox boxflags=TCTB_BOX, bool persistent=false);
        int color(int text,Color* col, Float alpha,TypeColorTBox boxflags=TCTB_BOX, bool persistent=false);


        int render();

        void clear();
    private:
        Writer();
        ~Writer();

        struct WriterImpl;
        Pimpl<WriterImpl> _impl;
    
        friend class Screen;
    };

    #ifdef GLOBAL_SINGLETON_REFERENCES
    extern Writer& Write;
    #endif

} // flatscene

#endif
