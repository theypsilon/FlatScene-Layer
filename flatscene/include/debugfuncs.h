#include "FSLibrary.h"

namespace flatscene {

    int getAlphaValues(Uint8* values,SDL_Surface* surface, SDL_Rect* rect);
    int fillAlphaValues(Uint8* values, SDL_Surface* surface);

    Uint32 getpixel(SDL_Surface* surface, int x, int y);
    void setalpha ( SDL_Surface* pSurface , int x , int y , Uint8 value);

    void countalpha(SDL_Surface* surface);

    int blitcopy(const SDL_Surface& ref_src, SDL_Rect* srcrect,SDL_Surface* surface,SDL_Rect* rect);

    void setpixel ( SDL_Surface* pSurface , int x , int y , Uint32 pixel);

}
