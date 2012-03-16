#include "FSCanvasImpl.h"
#include "FSLibrary.h"
#include "FSScreenImpl.h"

#include "debugfuncs.h"

#include <exception>

namespace flatscene {

Canvas::Canvas() {}

Canvas::Canvas( const SCanvas& canvas )
: bpp(canvas.bpp)
, h(canvas.h)
, h2(canvas.h2)
, w(canvas.w)
, w2(canvas.w2)
, tex(canvas.tex)
, sdl_surf(canvas.sdl_surf)
{}

Canvas::Canvas( Canvas&& pSurface ) 
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

Canvas::~Canvas( ) {

    if (sdl_surf) 
        SDL_FreeSurface(sdl_surf);

    if (h != 0 || w !=0)
        glDeleteTextures( 1, &tex );

    clearSurface();
}

void Canvas::clearSurface ( ) {
    bpp = h = h2 = w = w2 = tex = 0;
    sdl_surf = nullptr;
}

Uint32 Canvas::getPixel ( int x , int y ) const {
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
    return ( w2 );
}

int Canvas::getHeight () const {
    return ( h2 ) ;
}
void Canvas::put ( const FloatPoint& ptDst, Uint8 flags) const {
#ifdef MAINRENDERLOOP

    //PUSHMATRIX

    Screen::I().pushMatrix();

    //TRANSLATE

    Screen::I().translate(ptDst.x,ptDst.y,0);

    // USER DEFINED EFFECTS IN

    for (std::list<std::function<void()>>::const_iterator iri = initCallbackList.begin(),ire = initCallbackList.end(); iri != ire; ++iri) {
        (*iri)();
    }

    initCallbackList.clear();

    // PAINT FLOATCANVAS

    Screen::I()._impl->graphicMaterial.push_back(
        new Screen::ScreenImpl::SRenderCanvas(*this,flags)
    );

    // USER DEFINED EFFECTS OUT

    for (std::list<std::function<void()>>::const_iterator iri = endCallbackList.begin(),ire = endCallbackList.end(); iri != ire; ++iri) {
        (*iri)();
    }
     
    endCallbackList.clear();

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

    for (std::list<std::function<void()>>::const_iterator iri = initCallbackList.begin(),ire = initCallbackList.end(); iri != ire; ++iri) {
        (*iri)();
    }

    // PAINT FLOATCANVAS

    Screen::I()._impl->graphicMaterial.push_back(
        new Screen::ScreenImpl::SRenderCanvas(*this,flags)
    );

    // USER DEFINED EFFECTS OUT

    for (std::list<std::function<void()>>::const_iterator iri = endCallbackList.begin(),ire = endCallbackList.end(); iri != ire; ++iri) {
        (*iri)();
    }

    endCallbackList.clear();

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

        ret = SDL_CreateRGBSurface(s_surf->flags,s_surf->w*factor,s_surf->h*factor,bpp*8,s_surf->format->Rmask,s_surf->format->Gmask,s_surf->format->Bmask,s_surf->format->Amask);

    else if (bpp == 1) // 8 bits

        ret = SDL_CreateRGBSurface(s_surf->flags,s_surf->w*factor,s_surf->h*factor,bpp*8,0,0,0,0);

    else {
        Library::I().Error("depth mode not valid",TE_SDL_NOMSG);
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

Canvas Canvas::toSCanvas( SDL_Surface* surface, Uint8 mode, GLint filter) {

    if (pow2(mode) != mode)
        Library::I().Error("CCanvas::LoadIMG -> modo erroneo.");

    Canvas pSurface;

    SDL_Surface* image;
    SDL_Rect area;

    if (surface == nullptr) {
        Library::I().Error("CCanvas::LoadIMG -> image Null.");
        pSurface.w = pSurface.h = pSurface.bpp = pSurface.w2 = pSurface.h2 = pSurface.tex = 0;
        pSurface.sdl_surf = nullptr;
        return pSurface;
    }
    
    pSurface.w2 = surface->w;
    pSurface.h2 = surface->h;
    pSurface.tex = 0;
    pSurface.w = pow2((Uint32)surface->w);
    pSurface.h = pow2((Uint32)surface->h);
    pSurface.bpp = surface->format->BytesPerPixel;

    if (mode == ONLY_TEXTURE ||     mode == WITH_SDL_SURFACE) {
        int saved_flags;
        int  saved_alpha;
      
        #if SDL_BYTEORDER == SDL_LIL_ENDIAN
              image = SDL_CreateRGBSurface(
                  SDL_SWSURFACE |SDL_SRCALPHA,
                  pSurface.w,
                  pSurface.h,
                  surface->format->BitsPerPixel,
                  0x000000ff,
                  0x0000ff00,
                  0x00ff0000,
                  0xff000000);
        #else
              image = SDL_CreateRGBSurface(
                  SDL_SWSURFACE |SDL_SRCALPHA,
                  pSurface.w,
                  pSurface.h,
                  surface->format->BitsPerPixel,
                  0xff000000,
                  0x00ff0000,
                  0x0000ff00,
                  0x000000ff);
        #endif
        if (image == NULL) {
            Library::I().Error("CCanvas::LoadIMG -> image Null.");
            return pSurface;
        }


        saved_flags = surface->flags&(SDL_SRCALPHA|SDL_RLEACCELOK);
        saved_alpha = surface->format->alpha;
        if ( (saved_flags & SDL_SRCALPHA)   == SDL_SRCALPHA ) {
          SDL_SetAlpha(surface, 0, 0);
        }



        area.x = 0;
        area.y = 0;
        area.w = surface->w;
        area.h = surface->h;

        SDL_BlitSurface(surface, &area, image, &area);

        
        if ( (saved_flags & SDL_SRCALPHA)== SDL_SRCALPHA )  {
            SDL_SetAlpha(surface, saved_flags, saved_alpha);
        }

        //


        /*
        SDL_Surface* lpTexture = IMG_Load(szFilename);
        if (!lpTexture) {       // some error
            return;
        }

        m_iTextureWidth = lpTexture->w;
        m_iTextureHeight = lpTexture->h;
        glGenTextures(1, &m_iglTexture);
        rval = glGetError();
        if (rval != GL_NO_ERROR)
        {
            if (lpTexture)
            {
                SDL_FreeSurface(lpTexture);
                lpTexture = NULL;
            }
            // some error
            return;
        }
        glBindTexture(GL_TEXTURE_2D, m_iglTexture);
        rval = glGetError();
        if (rval != GL_NO_ERROR)
        {
            if (lpTexture)
            {
                SDL_FreeSurface(lpTexture);
                lpTexture = NULL;
            }
            // some error
            return;
        }
        int byte = 0;
        int w = lpTexture->w, h = lpTexture->h;
        int iTotalBytes = w * h * 4;
        unsigned char* lpNewTexture = new unsigned char[iTotalBytes];
        if (!lpNewTexture)  {
            if (lpTexture)      {
                SDL_FreeSurface(lpTexture);
                lpTexture = NULL;
            }           // some error
            return;
        }
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++)     {
                Uint8 r,g,b,a;
                Uint32 color = GetPixel(lpTexture, x, y);
                if(!bUseColorKey)           {
                    SDL_GetRGB(color, lpTexture->format, &r, &g, &b);
                    a = 0xff;
                }           else            {
                    SDL_GetRGBA(color, lpTexture->format, &r, &g, &b, &a);
                    if ((r == iColorKeyRed) && (g == iColorKeyGreen) && (b == iColorKeyBlue))               {
                        a = 0x00;
                    }               else                {
                        a = 0xff;
                    }
                }
                lpNewTexture[byte++] = r;
                lpNewTexture[byte++] = g;
                lpNewTexture[byte++] = b;
                lpNewTexture[byte++] = a;
            }
        }*/

        /*
        Uint8* line = new Uint8[image->pitch];
        Uint8* pixels = static_cast<Uint8*>(image->pixels);
        Uint16 pitch = image->pitch;
        int ybegin = 0;
        int yend = image->h - 1;

        if(SDL_MUSTLOCK(image))
            SDL_LockSurface(image);
        while(ybegin < yend)
        {
            memcpy(line, pixels + pitch * ybegin, pitch);
            memcpy(pixels + pitch * ybegin, pixels + pitch * yend, pitch);
            memcpy(pixels + pitch * yend, line, pitch);
            ybegin++;
            yend--;
        }

        if (line)
            delete[] line;

        */
        if(SDL_MUSTLOCK(image))
            SDL_UnlockSurface(image);

        // Have OpenGL generate a texture object handle for us
        glGenTextures(1, &pSurface.tex );

        // Bind the texture object
        glBindTexture( GL_TEXTURE_2D,pSurface.tex );
        
        // Set the texture's stretching properties
        if (filter != GL_NEAREST || filter != GL_LINEAR)
            filter = GL_NEAREST;

        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,filter);
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,filter);
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
        
        // Edit the texture object's surface data using the information SDL_Surface gives us
        glTexImage2D( GL_TEXTURE_2D,
                                0,
                                GL_RGBA,
                                pSurface.w,
                                pSurface.h,
                                0,
                                GL_RGBA,
                                GL_UNSIGNED_BYTE,
                                image->pixels );

        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

        if ( image ) {
            SDL_FreeSurface( image );
            image=NULL;
        }



    }

    if (mode & ONLY_SDL_SURFACE) {
        pSurface.h=0;
        pSurface.w=0;
    }

    if (mode & ONLY_TEXTURE) {
        SDL_FreeSurface( surface );
        surface=NULL;
    }

    pSurface.sdl_surf=surface;

    return pSurface;
}


Uint32 Canvas::pow2 (Uint32 n) {
    Uint32 c=1;
    while (c < n) 
        c<<=1;

    return c;
}

int Canvas::rotate(Float angle, Float x, Float y, Float z) const {

    initCallbackList.push_back([=](){
        Screen::I().rotate(angle,x,y,z);
    });

    return EXITO;
}
int Canvas::translate(Float x, Float y, Float z) const {

    initCallbackList.push_back([=](){
        Screen::I().translate(x,y,z);
    });

    return EXITO;
}
int Canvas::scale(Float x, Float y, Float z) const {

    initCallbackList.push_back([=](){
        Screen::I().scale(x,y,z);
    });

    return EXITO;
}

int Canvas::color(Float red, Float green, Float blue, Float alpha) const {

    if (red > 1.0) red = 1.0;
    if (green > 1.0) green = 1.0;
    if (blue > 1.0) blue = 1.0;
    if (alpha > 1.0) alpha = 1.0;

    initCallbackList.push_back([=](){
        Screen::I().color(red,green,blue,alpha);
    });

    red = Screen::I()._impl->red;//2.0 - red;
    green = Screen::I()._impl->green;//2.0 - green;
    blue = Screen::I()._impl->blue;//2.0 - blue;
    alpha =  Screen::I()._impl->alpha;//2.0 - alpha;

    endCallbackList.push_back([=](){
        Screen::I().color(red,green,blue,alpha);
    });

    return EXITO;
}

int Canvas::color(Color* col, Float alpha) const {
    return color(((Float)col->getR())/255.0,((Float)col->getG())/255.0,((Float)col->getB())/255.0,alpha);
}

} // flatscene
