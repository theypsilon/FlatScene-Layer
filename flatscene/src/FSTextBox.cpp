#include "FSWriterImpl.h"
#include "FSTime.h"
#include "SDL.h"
#include "FSScreenImpl.h"

#include "FSLibrary.h"

#define MARGEN 20

namespace flatscene {

Writer::WriterImpl::FSTextBox::FSTextBox(const char* file,const char* text,int x,int y,int Lim,SFont* ttf_fnt,int next) :
file(file), fuente(ttf_fnt), next(next), upleft(x,y), fx(nullptr), box(nullptr),
timer(Chrono.getTick()), step(0), maxStep(0)    {

    std::string allText(text);
    Uint16 limite=xBox=Lim;

    Float currentX = (Float) upleft.x+MARGEN;
    Float currentY = (Float) upleft.y+MARGEN-2;
    currentY += TTF_FontAscent(fuente->fuente)/2 ;

    col.r=255;
    col.b=255;
    col.g=255;

    size_t length = allText.length();

    while (length > 0 ) {

        Uint16 newChar = allText.c_str()[0];

        if ((newChar & 0x80)  && (length  > 1)) {

            if ((newChar & 0x20) && (length  > 2)) {

                allText =allText.substr(3,length);

            } else {


                newChar =  ((newChar & 0x1F) << 6) + ((allText.c_str()[1]) & 0x3F);

                allText =allText.substr(2,length);
            }

        } else {
            allText =allText.substr(1,length);
        }

        length = allText.length();

        int minx,maxy,advance;

        if (TTF_GlyphMetrics(fuente->fuente,newChar,&minx,NULL,NULL,&maxy,&advance)== -1)
            Library::I().Error("TTF_GlyphMetrics fallo.");

        if (newChar == ' ' ) {
            const char* caux = allText.c_str();
            Float cuenta = (Float)advance;

            for (int i=0;caux[i]!='\0' && caux[i]!=' ' && caux[i]!='\n';i++) {

                if (TTF_GlyphMetrics(fuente->fuente,caux[i],NULL,NULL,NULL,NULL,&minx) == -1)
                    Library::I().Error("TTF_GlyphMetrics fallo.");

                cuenta += (Float)minx;

            }

            if ((currentX + cuenta - x)>= (limite - MARGEN)) {
                currentX = (Float) upleft.x + MARGEN;
                currentY += (Float)TTF_FontLineSkip(fuente->fuente);
            } else {
                maxStep++;
                currentX += (Float)advance;
                SChar newT;

                charInDisplay.push_back(newT);
            }

        } else if (newChar == '\n') {
            currentX = (Float) upleft.x + MARGEN;
            currentY += (Float)TTF_FontLineSkip(fuente->fuente);
        } else {
            maxStep++;

            SChar newT;

            newT.p.set(currentX+(Float)minx,currentY-(Float)maxy);
            currentX += (Float)advance;

            newT.glyph=newChar;

            charInDisplay.push_back(newT);
        }

    }

     currentY -= (Float)y;

     currentY += (Float)TTF_FontLineSkip(fuente->fuente);

    yBox = (Uint8) currentY;

    createBox();

}

Writer::WriterImpl::FSTextBox::~FSTextBox() {
    deleteBox();
    Writer::I().unloadFont(Writer::I().searchFont(fuente->fuente));
}

int Writer::WriterImpl::FSTextBox::update() {

    if (fx && ( fx->boxflags == TCTB_ALL || fx->boxflags == TCTB_BOX )) {
        box->color(fx->red,fx->green,fx->blue,fx->alpha);
    }

    box->put(upleft);

    if ((Chrono.getTick() > timer + 3) && (step <= maxStep)) {

        timer = Chrono.getTick();

        step++;

    } else if ( Chrono.getTick() > timer + 100) {
        charInDisplay.clear();
        return -1;
    }

    unsigned int i=0;
    for (std::list<SChar>::iterator it=charInDisplay.begin(), et=charInDisplay.end();it!=et && i<step;++it) {
        if (fx && ( fx->boxflags == TCTB_ALL || fx->boxflags == TCTB_TEXT ))
            fuente->render.at(it->glyph).color(fx->red,fx->green,fx->blue,fx->alpha);
        fuente->render.at(it->glyph).put(it->p);
        i++;
    }

    if (fx && !fx->persistent) {
        fx.reset(nullptr);
    }

    return 0;

}

void Writer::WriterImpl::FSTextBox::deleteBox() {
    if (box)
        Screen::I()._impl->imageToDelete.push_back(box); // delete box;
    box=NULL;
}



void Writer::WriterImpl::FSTextBox::createBox() {
    if (box) {
        Library::I().Error("Ya existe el fondo de la caja que se pretende crear.");
        return;
    }

    SDL_Surface *surface, *aux_surf;

    aux_surf = SDL_CreateRGBSurface(0,xBox,yBox,Screen::I().getBpp(),0,0,255,0);
    if (!aux_surf)  Library::I().Error("No se ha creado bien la superficie para la TextBox.");
    surface = SDL_DisplayFormat(aux_surf);
    if (!surface)   Library::I().Error("No se ha creado bien la superficie para la TextBox.");
    SDL_FreeSurface(aux_surf);
    SDL_FillRect(surface,NULL,SDL_MapRGB(surface->format,50,50,150));

    box = new Canvas(Canvas::createCanvas<Canvas>(surface));
}

int Writer::WriterImpl::FSTextBox::finish() {
    int ret = -1;
    if (next!=-1) {

        ret = Writer::I().inBox(file.c_str(),next);
        if (fx && fx->persistent) {
            Writer::I().color(ret,fx->red,fx->green,fx->blue,fx->alpha,fx->boxflags,true);
        }
        if (ret == -1) {
            ; // ODOT : Quitar el error de la cola.
        }

    }

    return ret;
}

Writer::WriterImpl::SLineText::~SLineText() {}

} // flatscene
