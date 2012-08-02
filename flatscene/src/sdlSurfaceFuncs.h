#ifndef FS_SDL_SURFACE_FUNCS__
#define FS_SDL_SURFACE_FUNCS__

#include "CanvasResource.h"

namespace FlatScene {

    void setpixel ( SDL_Surface* pSurface , int x , int y , Uint32 pixel);

    Uint32 getpixel(SDL_Surface* surface, int x, int y);

    void countalpha(SDL_Surface* surface);

    void setalpha ( SDL_Surface* pSurface , int x , int y , Uint8 value);

    int getAlphaValues(Uint8* values,SDL_Surface* surface, SDL_Rect* rect);

    int fillAlphaValues(Uint8* values, SDL_Surface* surface);

    int blitcopy(const SDL_Surface& ref_src, SDL_Rect* srcrect,SDL_Surface* surface,SDL_Rect* rect);

    SDL_Surface* scaleSurface( SDL_Surface* s_surf, int factor);

    SDL_Surface* loadSurface(const SDL_Rect& src, ConstRawImageResource chipset, GraphicMode mode, double sp_scale);

    void storeSurface(CanvasResource& canvas, SDL_Surface* surface, GraphicMode mode, GraphicFilter filter);

    SDL_Surface* IMGLoadOrThrow(const std::string& path);

    void IMGFreeOrThrow(SDL_Surface* surface);

    inline unsigned int getWidth(const SDL_Surface& surface) {
        return surface.w;
    }

    inline unsigned int getHeight(const SDL_Surface& surface) {
        return surface.h;
    }

}

#endif // FS_SDL_SURFACE_FUNCS__