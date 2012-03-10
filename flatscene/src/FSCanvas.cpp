#include "FSCanvas.h"
#include "FSLibrary.h"
#include "FSScreenImpl.h"

#include "debugfuncs.h"

#include <exception>

FSCanvas::FSCanvas() {}

FSCanvas::FSCanvas( const SCanvas& canvas )
: bpp(canvas.bpp)
, h(canvas.h)
, h2(canvas.h2)
, w(canvas.w)
, w2(canvas.w2)
, tex(canvas.tex)
, sdl_surf(canvas.sdl_surf)
{}

FSCanvas::FSCanvas( FSCanvas&& pSurface ) 
: endCallbackList(std::move(pSurface.endCallbackList))
, initCallbackList(std::move(pSurface.initCallbackList)) 
, bpp(pSurface.bpp)
, h(pSurface.h)
, h2(pSurface.h2)
, w(pSurface.w)
, w2(pSurface.w2)
, tex(pSurface.tex)
, sdl_surf(pSurface.sdl_surf) {
    pSurface.clearSurface();
}

FSCanvas::~FSCanvas( ) {

    if (sdl_surf) 
        SDL_FreeSurface(sdl_surf);

    if (h != 0 || w !=0)
        glDeleteTextures( 1, &tex );

    clearSurface();
}

void FSCanvas::clearSurface ( ) {
    bpp = h = h2 = w = w2 = tex = 0;
    sdl_surf = nullptr;
}

Uint32 FSCanvas::getPixel ( int x , int y ) const {
    if (sdl_surf && sdl_surf->w > x && sdl_surf->h > y) {
        Uint32 color = 0 ;
        int position = y * sdl_surf->pitch + sdl_surf->format->BytesPerPixel * x ;
        char* buffer = ( char* ) sdl_surf->pixels ;
        buffer += position ;
        memcpy ( &color , buffer , sdl_surf->format->BytesPerPixel ) ;
        return ( color ) ;
    } else {
        return 0;
    }
}

int FSCanvas::getWidth () const {
    return ( w2 );
}

int FSCanvas::getHeight () const {
    return ( h2 ) ;
}
void FSCanvas::put ( const FSFloatPoint& ptDst, Uint8 flags) const {
#ifdef MAINRENDERLOOP

    //PUSHMATRIX

    FSScreen::I().pushMatrix();

    //TRANSLATE

    FSScreen::I().translate(ptDst.x,ptDst.y,0);

    // USER DEFINED EFFECTS IN

    for (std::list<std::function<void()>>::const_iterator iri = initCallbackList.begin(),ire = initCallbackList.end(); iri != ire; ++iri) {
        (*iri)();
    }

    initCallbackList.clear();

    // PAINT FLOATCANVAS

    FSScreen::I()._impl->graphicMaterial.push_back(
        new FSScreen::ScreenImpl::SRenderCanvas(*this,flags)
    );

    // USER DEFINED EFFECTS OUT

    for (std::list<std::function<void()>>::const_iterator iri = endCallbackList.begin(),ire = endCallbackList.end(); iri != ire; ++iri) {
        (*iri)();
    }
     
    endCallbackList.clear();

    // POPMATRIX

    FSScreen::I().popMatrix();

#else

    if (m_pSurface.h != 0 || m_pSurface.w !=0 ) {
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, m_pSurface.tex);

        glTranslatef((Float)ptDst.x,(Float)ptDst.y,0);

        Float relW = (Float)m_pSurface.w2/(Float)m_pSurface.w;
        Float relH = (Float)m_pSurface.h2/(Float)m_pSurface.h;

        #ifdef TEXTURE_BASE_SCALE
                glScalef((1.0/TEXTURE_BASE_SCALE ),(1.0/TEXTURE_BASE_SCALE ),0.0);
        #endif

        glBegin(GL_QUADS);
            if (flags == 0) {
                glTexCoord2f(0.0f, relH);
                glVertex2f(0, m_pSurface.h2);
                glTexCoord2f(relW, relH);
                glVertex2f(m_pSurface.w2, m_pSurface.h2);
                glTexCoord2f(relW, 0.0f);
                glVertex2f(m_pSurface.w2, 0);
                glTexCoord2f(0.0f, 0.0f);
                glVertex2f(0,0);
            } else if (flags == 1) {

                glTexCoord2f(relW, relH);
                glVertex2f(0, m_pSurface.h2);
                glTexCoord2f(0.0f, relH);
                glVertex2f(m_pSurface.w2, m_pSurface.h2);
                glTexCoord2f(0.0f, 0.0f);
                glVertex2f(m_pSurface.w2, 0);
                glTexCoord2f(relW, 0.0f);
                glVertex2f(0,0);
            } else if (flags==2) {
                glTexCoord2f(0.0f, 0.0f);
                glVertex2f(0, m_pSurface.h2);
                glTexCoord2f(relW, 0.0f);
                glVertex2f(m_pSurface.w2, m_pSurface.h2);
                glTexCoord2f(relW, relH);
                glVertex2f(m_pSurface.w2, 0);
                glTexCoord2f(0.0f, relH);
                glVertex2f(0,0);
            } else  {
                glTexCoord2f(relW, 0.0f);
                glVertex2f(0, m_pSurface.h2);
                glTexCoord2f(0.0f, 0.0f);
                glVertex2f(m_pSurface.w2, m_pSurface.h2);
                glTexCoord2f(0.0f, relH);
                glVertex2f(m_pSurface.w2, 0);
                glTexCoord2f(relW, relH);
                glVertex2f(0,0);
            }
        glEnd();

        glPopMatrix();
    }

#endif
}
void FSCanvas::put ( const FSPoint& ptDst, Uint8 flags) const {
#ifdef MAINRENDERLOOP

    //PUSHMATRIX

    FSScreen::I().pushMatrix();

    //TRANSLATE

    FSScreen::I().translate(ptDst.x,ptDst.y,0);

    // USER DEFINED EFFECTS IN

    for (std::list<std::function<void()>>::const_iterator iri = initCallbackList.begin(),ire = initCallbackList.end(); iri != ire; ++iri) {
        (*iri)();
    }

    // PAINT FLOATCANVAS

    FSScreen::I()._impl->graphicMaterial.push_back(
        new FSScreen::ScreenImpl::SRenderCanvas(*this,flags)
    );

    // USER DEFINED EFFECTS OUT

    for (std::list<std::function<void()>>::const_iterator iri = endCallbackList.begin(),ire = endCallbackList.end(); iri != ire; ++iri) {
        (*iri)();
    }

    endCallbackList.clear();

    // POPMATRIX

    FSScreen::I().popMatrix();

#else

    if (m_pSurface.h != 0 || m_pSurface.w !=0 ) {
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, m_pSurface.tex);

        glTranslatef((Float)ptDst.x,(Float)ptDst.y,0);

        Float relW = (Float)m_pSurface.w2/(Float)m_pSurface.w;
        Float relH = (Float)m_pSurface.h2/(Float)m_pSurface.h;

        #ifdef TEXTURE_BASE_SCALE
                glScalef((1.0/TEXTURE_BASE_SCALE ),(1.0/TEXTURE_BASE_SCALE ),0.0);
        #endif

        glBegin(GL_QUADS);
            if (flags == 0) {
                glTexCoord2f(0.0f, relH);
                glVertex2f(0, m_pSurface.h2);
                glTexCoord2f(relW, relH);
                glVertex2f(m_pSurface.w2, m_pSurface.h2);
                glTexCoord2f(relW, 0.0f);
                glVertex2f(m_pSurface.w2, 0);
                glTexCoord2f(0.0f, 0.0f);
                glVertex2f(0,0);
            } else if (flags == 1) {

                glTexCoord2f(relW, relH);
                glVertex2f(0, m_pSurface.h2);
                glTexCoord2f(0.0f, relH);
                glVertex2f(m_pSurface.w2, m_pSurface.h2);
                glTexCoord2f(0.0f, 0.0f);
                glVertex2f(m_pSurface.w2, 0);
                glTexCoord2f(relW, 0.0f);
                glVertex2f(0,0);
            } else if (flags==2) {
                glTexCoord2f(0.0f, 0.0f);
                glVertex2f(0, m_pSurface.h2);
                glTexCoord2f(relW, 0.0f);
                glVertex2f(m_pSurface.w2, m_pSurface.h2);
                glTexCoord2f(relW, relH);
                glVertex2f(m_pSurface.w2, 0);
                glTexCoord2f(0.0f, relH);
                glVertex2f(0,0);
            } else  {
                glTexCoord2f(relW, 0.0f);
                glVertex2f(0, m_pSurface.h2);
                glTexCoord2f(0.0f, 0.0f);
                glVertex2f(m_pSurface.w2, m_pSurface.h2);
                glTexCoord2f(0.0f, relH);
                glVertex2f(m_pSurface.w2, 0);
                glTexCoord2f(relW, relH);
                glVertex2f(0,0);
            }
        glEnd();

        glPopMatrix();
    }

#endif
}

SDL_Surface* FSCanvas::scaleSurface( SDL_Surface* s_surf, int factor) {

    SDL_Surface* ret = NULL;

    if (s_surf == NULL || factor <= 1)
        return ret;

    Uint8 bpp = s_surf->format->BytesPerPixel;

    if (bpp == 4) // 32 bits

        ret = SDL_CreateRGBSurface(s_surf->flags,s_surf->w*factor,s_surf->h*factor,bpp*8,s_surf->format->Rmask,s_surf->format->Gmask,s_surf->format->Bmask,s_surf->format->Amask);

    else if (bpp == 1) // 8 bits

        ret = SDL_CreateRGBSurface(s_surf->flags,s_surf->w*factor,s_surf->h*factor,bpp*8,0,0,0,0);

    else {
        FSLibrary::I().Error("depth mode not valid",TE_SDL_NOMSG);
        return ret;

    }

    char* newPixels = (char*) ret->pixels;
    char* oldPixels = (char*) s_surf->pixels;

    SDL_LockSurface(ret);
    SDL_LockSurface(s_surf);

    for (int y=0;y<s_surf->h;y++) {

        for (int x=0;x<s_surf->w;x++) {

            int pos_old = y * s_surf->pitch + x * bpp;

            for (int fx = 0 ; fx<factor;fx++) {
                for (int fy = 0 ; fy < factor ; fy++) {

                    int pos_new = (y*factor + fy) * ret->pitch + (x*factor +fx) * bpp;

                    for (int b=0;b<bpp;b++) {

                        newPixels[pos_new+b]=oldPixels[pos_old+b];
                    }
                }
            }

        }

    }

    SDL_UnlockSurface(s_surf);
    SDL_UnlockSurface(ret);

    return ret;
}

Uint32 FSCanvas::pow2 (Uint32 n) {
    Uint32 c=1;
    while (c < n) 
        c<<=1;

    return c;
}

int FSCanvas::rotate(Float angle, Float x, Float y, Float z) const {

    initCallbackList.push_back([=](){
        FSScreen::I().rotate(angle,x,y,z);
    });

    return EXITO;
}
int FSCanvas::translate(Float x, Float y, Float z) const {

    initCallbackList.push_back([=](){
        FSScreen::I().translate(x,y,z);
    });

    return EXITO;
}
int FSCanvas::scale(Float x, Float y, Float z) const {

    initCallbackList.push_back([=](){
        FSScreen::I().scale(x,y,z);
    });

    return EXITO;
}

int FSCanvas::color(Float red, Float green, Float blue, Float alpha) const {

    if (red > 1.0) red = 1.0;
    if (green > 1.0) green = 1.0;
    if (blue > 1.0) blue = 1.0;
    if (alpha > 1.0) alpha = 1.0;

    initCallbackList.push_back([=](){
        FSScreen::I().color(red,green,blue,alpha);
    });

    red = FSScreen::I()._impl->red;//2.0 - red;
    green = FSScreen::I()._impl->green;//2.0 - green;
    blue = FSScreen::I()._impl->blue;//2.0 - blue;
    alpha =  FSScreen::I()._impl->alpha;//2.0 - alpha;

    endCallbackList.push_back([=](){
        FSScreen::I().color(red,green,blue,alpha);
    });

    return EXITO;
}

int FSCanvas::color(FSColor* col, Float alpha) const {
    return color(((Float)col->getR())/255.0,((Float)col->getG())/255.0,((Float)col->getB())/255.0,alpha);
}
