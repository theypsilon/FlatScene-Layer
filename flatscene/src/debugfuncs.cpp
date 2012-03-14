#include "debugfuncs.h"

using namespace flatscene;

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
        return FRACASO;

    for (int x = rect->x; x < rect->w; x++)
        for (int y = rect->y ; y < rect->h ; y++) {

            Uint32 pixel = getpixel(surface,x,y);

            values[i] = pixel >> 6;

            i++;

        }

    return EXITO;

}

int fillAlphaValues(Uint8* values, SDL_Surface* surface) {

    int i=0;

    if (values == NULL || surface == NULL)
        return FRACASO;

    for (int x = 0; x < surface->w; x++)
        for (int y = 0 ; y < surface->h ; y++) {

            //setalpha(surface,x,y,values[i]);
            i++;

//          printf("%08X ",getpixel(surface,x,y));

        }

    return FRACASO;
}

int blitcopy(const SDL_Surface& ref_src, SDL_Rect* srcrect,SDL_Surface* surface,SDL_Rect* rect) {
    int ancho = 0, alto = 0, srcx = 0, srcy = 0, dstx = 0, dsty = 0;

    SDL_Surface* src = const_cast<SDL_Surface*>(std::addressof(ref_src));

    if (src == NULL || surface == NULL)
        return FRACASO;

    if (srcrect == NULL && rect == NULL) {
        if (src->w > surface->w)
            ancho = surface->w;
        else
            ancho = src->w;

        if (src->h > surface->h)
            alto = surface->h;
        else
            alto = src->h;

        dstx = dsty = srcx = srcy = 0;

    } else if (srcrect == NULL) {
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

    } else if (rect == NULL) {
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
        return FRACASO;

    if (dstx + ancho > surface->w)
        return FRACASO;

    if (srcy + alto > src->h)
        return FRACASO;

    if (dsty + alto > surface->h)
        return FRACASO;

    Uint32* pixel = new Uint32[ancho*alto];

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

    return EXITO;
}

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
