#ifndef FS_CANVAS_FACTORY__
#define FS_CANVAS_FACTORY__

#include "sdlSurfaceFuncs.h"
#include "CanvasResource.h"

namespace FlatScene {

    unsigned inline int pow2(unsigned int n) {
        unsigned int c = 1;
        while (c < n) c <<= 1;
        return c;
    };

    template <class T> T createCanvas(
        const SDL_Rect& src, const SDL_Surface& chipset, 
        GraphicMode mode, double sp_scale, GraphicFilter filter
    ) {
        static_assert(
            /*std::is_trivially_constructible<T>::value && */
            std::is_base_of<Canvas,T>::value,
            "Bad Canvas type"
        );

        Point p(src.x,src.y);

        for (const auto& pair : Canvas::MemoryPolicyType::getCounts())
            if (pair.first->xy == p && pair.first->c == &chipset)
                return T(pair.first);

        T newCanvas(p,&chipset);
        SDL_Surface* surface = loadSurface(src,chipset,mode,sp_scale);

        CanvasResource& pSurface = static_cast<Canvas&>(newCanvas).getRes();

        SDL_Surface* image;
        SDL_Rect area;

        if (surface == nullptr)
            throw Exception("CCanvas::LoadIMG -> image Null.",__LINE__);
    
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
            if (image == NULL)
                throw Exception("CCanvas::LoadIMG -> image Null.",__LINE__);


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

            if(SDL_MUSTLOCK(image))
                SDL_UnlockSurface(image);

            // Have OpenGL generate a texture object handle for us
            glGenTextures(1, &pSurface.tex );

            // Bind the texture object
            glBindTexture( GL_TEXTURE_2D,pSurface.tex );

            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,filter == LINEAR ? GL_LINEAR : GL_NEAREST); //FIXME Provide more filters choices
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,filter == LINEAR ? GL_LINEAR : GL_NEAREST);
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

        return newCanvas;
    }

} // flatscene

#endif