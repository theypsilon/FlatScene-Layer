#include "sdlSurfaceFuncs.h"
#include "CanvasResource.h"

unsigned int pow2(unsigned int n) {
    unsigned int c = 1;
    while (c < n) c <<= 1;
    return c;
};

namespace FlatScene {

    void setpixel ( SDL_Surface* pSurface , int x , int y , Uint32 pixel) {

        if (SDL_MUSTLOCK(pSurface))
            SDL_LockSurface(pSurface);
        //determine position
        Uint32* pPosition = ( Uint32* ) pSurface->pixels ;

        //offset by y
        pPosition += ( pSurface->pitch * y / pSurface->format->BytesPerPixel) ;

        //offset by x
        pPosition += x;

        *pPosition = pixel;
        /*
        *pPosition = pixel & 0xFF;

        pPosition ++;

        *pPosition = (pixel & 0xFF00) >> 2;

        pPosition ++;

        *pPosition = (pixel & 0xFF0000) >> 4;

        pPosition ++;

        *pPosition = (pixel & 0xFF000000) >> 6;

        pPosition ++;*/

        if (SDL_MUSTLOCK(pSurface))
            SDL_UnlockSurface(pSurface);

    }

    Uint32 getpixel(SDL_Surface* surface, int x, int y) {

        if (surface && surface->w > x && surface->h > y) {

            if (SDL_MUSTLOCK(surface))
                SDL_LockSurface(surface);

            Uint32 color = 0 ;
            int position = y * surface->pitch + surface->format->BytesPerPixel * x ;
            char* buffer = ( char* ) surface->pixels ;
            buffer += position ;
            memcpy ( &color , buffer , surface->format->BytesPerPixel ) ;

            if (SDL_MUSTLOCK(surface))
                SDL_UnlockSurface(surface);

            return ( color ) ;
        } else {
            return 0;
        }
    }

    void countalpha(SDL_Surface* surface) {
        int numalpha = 0;
        int numsolid = 0;
        int numpurealpha = 0;
        int numblue = 0;
        int numblack = 0;

        Uint32 pixel = 0;

        //surface = IMG_Load("C:\\Programacion\\VS Workspace\\flatscene - lib\\demoark\\resources\\Character.png");

        for (int y=0; y< surface->h;y++) {
            for (int x = 0;x < surface->w;x++) {
                pixel = getpixel(surface,x,y);

                if ((pixel & 0xFF000000) < 255) {
                    numpurealpha++;
                }

                if ((pixel & 0x00FF0000) > 0) {
                    numalpha++;
                }

                if ((pixel & 0x0000FF00) > 0) {
                    numsolid++;
                }

                if ((pixel & 0x000000FF) > 0) {
                    numblue++;
                }

                if (pixel == 0) {
                    numblack++;
                }

                if (x==0) printf("\n");

                printf("%08X ",pixel);

            }

            break;

        }

        printf("alpha:%d red:%d green:%d blue:%d pureblack:%d\n",numpurealpha,numalpha,numsolid,numblue,numblack);

        exit(0);
    }

    void setalpha ( SDL_Surface* pSurface , int x , int y , Uint8 value)
    {

        //determine position
        Uint8* pPosition = ( Uint8* ) pSurface->pixels ;

        //offset by y
        pPosition += ( pSurface->pitch * y ) ;

        //offset by x
        pPosition += ( pSurface->format->BytesPerPixel * x ) ;

        pPosition += 3;

        *pPosition = value;
    }

    int getAlphaValues(Uint8* values,SDL_Surface* surface, SDL_Rect* rect) {

        int i=0;

        if (values == NULL || surface == NULL || rect == NULL)
            return EXIT_FAILURE;

        for (int x = rect->x; x < rect->w; x++)
            for (int y = rect->y ; y < rect->h ; y++) {

                Uint32 pixel = getpixel(surface,x,y);

                values[i] = pixel >> 6;

                i++;

            }

        return EXIT_SUCCESS;

    }

    int fillAlphaValues(Uint8* values, SDL_Surface* surface) {

        int i=0;

        if (values == NULL || surface == NULL)
            return EXIT_FAILURE;

        for (int x = 0; x < surface->w; x++)
            for (int y = 0 ; y < surface->h ; y++) {

                //setalpha(surface,x,y,values[i]);
                i++;

    //          printf("%08X ",getpixel(surface,x,y));

            }

        return EXIT_FAILURE;
    }

    int blitcopy(const SDL_Surface& ref_src, SDL_Rect* srcrect,SDL_Surface* surface,SDL_Rect* rect) {
        int ancho = 0, alto = 0, srcx = 0, srcy = 0, dstx = 0, dsty = 0;

        SDL_Surface* src = const_cast<SDL_Surface*>(std::addressof(ref_src));

        if (src == nullptr || surface == nullptr)
            return EXIT_FAILURE;

        if (srcrect == nullptr && rect == nullptr) {
            if (src->w > surface->w)
                ancho = surface->w;
            else
                ancho = src->w;

            if (src->h > surface->h)
                alto = surface->h;
            else
                alto = src->h;

            dstx = dsty = srcx = srcy = 0;

        } else if (srcrect == nullptr) {
            if (src->w > rect->w)
                ancho = rect->w;
            else
                ancho = src->w;

            if (src->h > rect->h)
                alto = rect->h;
            else
                alto = src->h;

            srcx = srcy = 0;
            dstx = rect->x;
            dsty = rect->y;

        } else if (rect == nullptr) {
            if (srcrect->w > surface->w)
                ancho = surface->w;
            else
                ancho = srcrect->w;

            if (srcrect->h > surface->h)
                alto = surface->h;
            else
                alto = srcrect->h;

            dstx = dsty = 0;
            srcx = srcrect->x;
            srcy = srcrect->y;

        } else {
            if (srcrect->w > rect->w)
                ancho = rect->w;
            else
                ancho = srcrect->w;

            if (srcrect->h > rect->h)
                alto = rect->h;
            else
                alto = srcrect->h;

            srcx = srcrect->x;
            srcy = srcrect->y;
            dstx = rect->x;
            dsty = rect->y;
        }

        if (srcx + ancho > src->w)
            return EXIT_FAILURE;

        if (dstx + ancho > surface->w)
            return EXIT_FAILURE;

        if (srcy + alto > src->h)
            return EXIT_FAILURE;

        if (dsty + alto > surface->h)
            return EXIT_FAILURE;

        Uint32* pixel = new Uint32[ancho*alto];
        assert(pixel);

        int count = 0;

        for (int y = srcy, j =srcy + alto ; y < j ; y++) {
            for (int x = srcx, i = srcx + ancho; x < i; x++) {

                pixel[count]= getpixel(src,x,y);

                count++;

            }
        }

        count = 0;

        for (int y = dsty, j = dsty + alto ; y < j ; y++) {
            for (int x = dstx, i = dstx + ancho; x < i; x++) {

                setpixel(surface,x,y,pixel[count]);

                count++;

            }
        }

        return EXIT_SUCCESS;
    }

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

    SDL_Surface* loadSurface(const SDL_Rect& src, const SDL_Surface& chipset, GraphicMode mode, double sp_scale) {
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

    void storeSurface(CanvasResource& canvas, SDL_Surface* surface, GraphicMode mode, GraphicFilter filter) {
        SDL_Surface* image;
        SDL_Rect area;

        if (surface == nullptr)
            throw Exception("CCanvas::LoadIMG -> image Null.",__LINE__);
    
        canvas.w2 = surface->w;
        canvas.h2 = surface->h;
        canvas.tex = 0;
        canvas.w = pow2((Uint32)surface->w);
        canvas.h = pow2((Uint32)surface->h);
        canvas.bpp = surface->format->BytesPerPixel;

        if (mode == ONLY_TEXTURE ||     mode == WITH_SDL_SURFACE) {
            int saved_flags;
            int  saved_alpha;
      
            #if SDL_BYTEORDER == SDL_LIL_ENDIAN
                  image = SDL_CreateRGBSurface(
                      SDL_SWSURFACE |SDL_SRCALPHA,
                      canvas.w,
                      canvas.h,
                      surface->format->BitsPerPixel,
                      0x000000ff,
                      0x0000ff00,
                      0x00ff0000,
                      0xff000000);
            #else
                  image = SDL_CreateRGBSurface(
                      SDL_SWSURFACE |SDL_SRCALPHA,
                      canvas.w,
                      canvas.h,
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
            glGenTextures(1, &canvas.tex );

            // Bind the texture object
            glBindTexture( GL_TEXTURE_2D,canvas.tex );

            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,filter == LINEAR ? GL_LINEAR : GL_NEAREST); //FIXME Provide more filters choices
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,filter == LINEAR ? GL_LINEAR : GL_NEAREST);
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
        
            // Edit the texture object's surface data using the information SDL_Surface gives us
            glTexImage2D( GL_TEXTURE_2D,
                                    0,
                                    GL_RGBA,
                                    canvas.w,
                                    canvas.h,
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
            canvas.h=0;
            canvas.w=0;
        }

        if (mode & ONLY_TEXTURE) {
            SDL_FreeSurface( surface );
            surface=NULL;
        }

        canvas.raw=surface;

    }

} // FlatScene