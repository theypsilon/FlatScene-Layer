#include "CanvasResource.h"
#include "ScreenImpl.h"
#include "Exception.h"
#include "Algorithm.h"

namespace FlatScene {

Canvas::Canvas() 
    : ResourceHandler(new CanvasResource)
{}

Canvas::Canvas(CanvasResource* res) 
    : ResourceHandler(res)
{}

unsigned int Canvas::getPixel ( int x , int y ) const {
    SDL_Surface* sdl_surf = getRes().sdl_surf;
    if (sdl_surf && sdl_surf->w > x && sdl_surf->h > y) {
        Uint32 color = 0 ;
        int position = y * sdl_surf->pitch + sdl_surf->format->BytesPerPixel * x ;
        char* buffer = (char*) sdl_surf->pixels ;
        buffer += position ;
        memcpy ( &color , buffer , sdl_surf->format->BytesPerPixel ) ;
        return color;
    } else 
        return 0;
}

int Canvas::getWidth  () const { return getRes().w2; }
int Canvas::getHeight () const { return getRes().h2; }

template <typename PointType, typename GraphicMaterial>
inline void putCanvas ( const PointType& ptDst, unsigned char flags, 
                        const CanvasResource& impl, GraphicMaterial& gm ) {

    Screen::I().pushMatrix();
    Screen::I().translate(ptDst.x,ptDst.y,0);

    // USER DEFINED EFFECTS IN

    call_to_all(impl.initCallbackList);
    impl.initCallbackList.clear();

    gm.push_back(
        new Screen::ScreenImpl::SRenderCanvas(impl,flags)
    );

    // USER DEFINED EFFECTS OUT

    call_to_all(impl.endCallbackList);
    impl.endCallbackList.clear();

    Screen::I().popMatrix();
}

void Canvas::put ( const FloatPoint& ptDst, unsigned char flags) const {
    putCanvas( ptDst, flags, getRes(), Screen::I()._impl->graphicMaterial );
}

void Canvas::put ( const Point& ptDst, unsigned char flags) const {
    putCanvas( ptDst, flags, getRes(), Screen::I()._impl->graphicMaterial );
}

SDL_Surface* Canvas::scaleSurface( SDL_Surface* s_surf, int factor) {

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

void Canvas::rotate(Float angle, Float x, Float y, Float z) const {
    getRes().initCallbackList.push_back([=](){
        Screen::I().rotate(angle,x,y,z);
    });
}
void Canvas::translate(Float x, Float y, Float z) const {
    getRes().initCallbackList.push_back([=](){
        Screen::I().translate(x,y,z);
    });
}

void Canvas::scale(Float x, Float y, Float z) const {
    getRes().initCallbackList.push_back([=](){
        Screen::I().scale(x,y,z);
    });
}

void Canvas::color(Float red, Float green, Float blue, Float alpha) const {

    if (red > 1.0) red = 1.0;
    if (green > 1.0) green = 1.0;
    if (blue > 1.0) blue = 1.0;
    if (alpha > 1.0) alpha = 1.0;

    getRes().initCallbackList.push_back([=](){
        Screen::I().color(red,green,blue,alpha);
    });

    red   = Screen::I()._impl->red  ; //2.0 - red;
    green = Screen::I()._impl->green; //2.0 - green;
    blue  = Screen::I()._impl->blue ; //2.0 - blue;
    alpha = Screen::I()._impl->alpha; //2.0 - alpha;

    getRes().endCallbackList.push_back([=](){
        Screen::I().color(red,green,blue,alpha);
    });
}

void Canvas::color(const Color& col, Float alpha) const {
    color(  ((Float) col.getR() ) / 255.0,
            ((Float) col.getG() ) / 255.0,
            ((Float) col.getB() ) / 255.0,
            alpha );
}

} // flatscene
