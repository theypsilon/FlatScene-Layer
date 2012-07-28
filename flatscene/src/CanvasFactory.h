#ifndef FS_CANVAS_FACTORY__
#define FS_CANVAS_FACTORY__

#include "Library.h"
#include "CanvasResource.h"

namespace FlatScene {

    SDL_Surface* scaleSurface( SDL_Surface* s_surf, int factor) {

        SDL_Surface* ret = NULL;

        if (s_surf == NULL || factor <= 1)
            return ret;

        unsigned char bpp = s_surf->format->BytesPerPixel;

        if (bpp == 4) // 32 bits
            ret = SDL_CreateRGBSurface(s_surf->flags, s_surf->w * factor, s_surf->h * factor, bpp*8,
                s_surf->format->Rmask, s_surf->format->Gmask,
                s_surf->format->Bmask, s_surf->format->Amask
            );
        else if (bpp == 1) // 8 bits
            ret = SDL_CreateRGBSurface(s_surf->flags, s_surf->w * factor, s_surf->h * factor, bpp*8, 0,0,0,0 );
        else 
            throw SDLException("depth mode not valid");

        

        char* newPixels = (char*) ret->pixels;
        char* oldPixels = (char*) s_surf->pixels;

        SDL_LockSurface(ret);
        SDL_LockSurface(s_surf);

        for (int y = 0; y < s_surf->h; y++) {
            for (int x = 0; x < s_surf->w; x++) {

                int pos_old = y * s_surf->pitch + x * bpp;

                for (int fx = 0; fx < factor; fx++) {
                    for (int fy = 0; fy < factor; fy++) {

                        int pos_new = (y*factor + fy) * ret->pitch + (x*factor +fx) * bpp;

                        for (int b = 0; b < bpp; b++)
                            newPixels [pos_new + b] =oldPixels [pos_old + b];
                    }
                }
            }
        }

        SDL_UnlockSurface(s_surf);
        SDL_UnlockSurface(ret);

        return ret;
    }

    SDL_Surface* loadSurface(const SDL_Rect& src, const SDL_Surface& chipset, unsigned char mode, double sp_scale) {
        auto surf = SDL_CreateRGBSurface(chipset.flags | SDL_SRCALPHA,
                                         src.w - src.x, src.h - src.y,
                                         chipset.format->BitsPerPixel,
                                         chipset.format->Rmask, chipset.format->Gmask,
                                         chipset.format->Bmask, chipset.format->Amask);

        SDL_SetColorKey(surf,SDL_SRCCOLORKEY, chipset.format->colorkey);
        blitcopy(chipset,const_cast<SDL_Rect*>(&src),surf,nullptr);

        if (sp_scale != 1.0 && mode != ONLY_SDL_SURFACE) {
            if (auto temp = scaleSurface(surf,(int)sp_scale)) {
                SDL_FreeSurface(surf);
                surf=temp;
            }
            // Reasignamos los formatos.
            SDL_SetColorKey(surf,SDL_SRCCOLORKEY,chipset.format->colorkey);
        }
        return surf;
    }

    template <class T> 
    T Canvas::createCanvas(
        const SDL_Rect& src, const SDL_Surface& chipset, 
        unsigned char mode, double sp_scale, GraphicFilter filter
    ) {
        static_assert(
            /*std::is_trivially_constructible<T>::value && */
            std::is_base_of<Canvas,T>::value,
            "Bad Canvas type"
        );

        auto pow2 = [](unsigned int n) {
            unsigned int c=1;
            while (c < n) 
                c<<=1;

            return c;
        };

        Point p(src.x,src.y);

        for (const auto& pair : Canvas::MemoryPolicyType::getCounts())
            if (pair.first->xy.x == p.x && pair.first->xy.y == p.y && pair.first->c == &chipset)
                return T(pair.first);

        T newCanvas(p,&chipset);
        SDL_Surface* surface = loadSurface(src,chipset,mode,sp_scale);

        if (pow2(mode) != mode)
            throw Exception("CCanvas::LoadIMG -> modo erroneo.",__LINE__);

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
