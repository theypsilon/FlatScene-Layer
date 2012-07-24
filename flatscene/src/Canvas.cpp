#include "CanvasImpl.h"
#include "Library.h"
#include "ScreenImpl.h"

#include "debugfuncs.h"

#include "Exception.h"

namespace FlatScene {

Canvas::Canvas() 
    : _impl(new CanvasImpl)
{}

Canvas::Canvas( Canvas&& pSurface ) 
    : _impl(std::move(pSurface._impl)){
}

Canvas::~Canvas( ) {}

Uint32 Canvas::getPixel ( int x , int y ) const {
    SDL_Surface* sdl_surf = _impl->sdl_surf;
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

int Canvas::getWidth () const {
    return ( _impl->w2 );
}

int Canvas::getHeight () const {
    return ( _impl->h2 ) ;
}
void Canvas::put ( const FloatPoint& ptDst, Uint8 flags) const {
#ifdef MAINRENDERLOOP

    //PUSHMATRIX

    Screen::I().pushMatrix();

    //TRANSLATE

    Screen::I().translate(ptDst.x,ptDst.y,0);

    // USER DEFINED EFFECTS IN

    for (const auto& callback : _impl->initCallbackList) {
        callback();
    }

    _impl->initCallbackList.clear();

    // PAINT FLOATCANVAS

    Screen::I()._impl->graphicMaterial.push_back(
        new Screen::ScreenImpl::SRenderCanvas(*this->_impl,flags)
    );

    // USER DEFINED EFFECTS OUT

    for (const auto& callback : _impl->endCallbackList) {
        callback();
    }
     
    _impl->endCallbackList.clear();

    // POPMATRIX

    Screen::I().popMatrix();

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
void Canvas::put ( const Point& ptDst, Uint8 flags) const {
#ifdef MAINRENDERLOOP

    //PUSHMATRIX

    Screen::I().pushMatrix();

    //TRANSLATE

    Screen::I().translate(ptDst.x,ptDst.y,0);

    // USER DEFINED EFFECTS IN

    for (const auto& callback : _impl->initCallbackList) {
        callback();
    }

    _impl->initCallbackList.clear();

    // PAINT FLOATCANVAS

    Screen::I()._impl->graphicMaterial.push_back(
        new Screen::ScreenImpl::SRenderCanvas(*this->_impl,flags)
    );

    // USER DEFINED EFFECTS OUT

    for (const auto& callback : _impl->endCallbackList) {
        callback();
    }

    _impl->endCallbackList.clear();

    // POPMATRIX

    Screen::I().popMatrix();

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

SDL_Surface* Canvas::scaleSurface( SDL_Surface* s_surf, int factor) {

    SDL_Surface* ret = NULL;

    if (s_surf == NULL || factor <= 1)
        return ret;

    Uint8 bpp = s_surf->format->BytesPerPixel;

    if (bpp == 4) // 32 bits
        ret = SDL_CreateRGBSurface(s_surf->flags,s_surf->w*factor,s_surf->h*factor,bpp*8,
            s_surf->format->Rmask,s_surf->format->Gmask,
            s_surf->format->Bmask,s_surf->format->Amask
        );
    else if (bpp == 1) // 8 bits
        ret = SDL_CreateRGBSurface(s_surf->flags,s_surf->w*factor,s_surf->h*factor,bpp*8,
            0,0,0,0
        );
    else 
        throw SDLException("depth mode not valid");

    

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

int Canvas::rotate(Float angle, Float x, Float y, Float z) const {

    _impl->initCallbackList.push_back([=](){
        Screen::I().rotate(angle,x,y,z);
    });

    return EXITO;
}
int Canvas::translate(Float x, Float y, Float z) const {

    _impl->initCallbackList.push_back([=](){
        Screen::I().translate(x,y,z);
    });

    return EXITO;
}
int Canvas::scale(Float x, Float y, Float z) const {

    _impl->initCallbackList.push_back([=](){
        Screen::I().scale(x,y,z);
    });

    return EXITO;
}

int Canvas::color(Float red, Float green, Float blue, Float alpha) const {

    if (red > 1.0) red = 1.0;
    if (green > 1.0) green = 1.0;
    if (blue > 1.0) blue = 1.0;
    if (alpha > 1.0) alpha = 1.0;

    _impl->initCallbackList.push_back([=](){
        Screen::I().color(red,green,blue,alpha);
    });

    red = Screen::I()._impl->red;//2.0 - red;
    green = Screen::I()._impl->green;//2.0 - green;
    blue = Screen::I()._impl->blue;//2.0 - blue;
    alpha =  Screen::I()._impl->alpha;//2.0 - alpha;

    _impl->endCallbackList.push_back([=](){
        Screen::I().color(red,green,blue,alpha);
    });

    return EXITO;
}

int Canvas::color(Color* col, Float alpha) const {
    return color(((Float)col->getR())/255.0,((Float)col->getG())/255.0,((Float)col->getB())/255.0,alpha);
}

} // flatscene
