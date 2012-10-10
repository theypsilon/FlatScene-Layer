#include "CanvasResource.h"
#include "Exception.h"
#include "Renders.h"
#include "ScreenImpl.h"
#include "sdlSurfaceFuncs.h"
#include <iostream>

inline unsigned int pow2(unsigned int n) {
    unsigned int c = 1;
    while (c < n) c <<= 1;
    return c;
};

namespace FlatScene {

    CanvasResource::~CanvasResource() {
        cleanResourcesCPU();
        cleanResourcesGPU();
    }

    void CanvasResource::cleanResourcesGPU() {
    }

    void CanvasResource::cleanResourcesCPU() {
    }

    template <typename PointType, typename GraphicMaterial>
    inline void putCanvas ( const PointType& ptDst, unsigned char flags, 
                            const CanvasResource& impl, GraphicMaterial& gm ) {

        Screen::I().pushMatrix();
        Screen::I().translate(ptDst.x,ptDst.y,0);

        // USER DEFINED EFFECTS IN

        call_to_all(impl.initCallbackList);
        impl.initCallbackList.clear();

        gm.push_back(
            new SRenderCanvas(impl,flags)
        );

        // USER DEFINED EFFECTS OUT

        call_to_all(impl.endCallbackList);
        impl.endCallbackList.clear();

        Screen::I().popMatrix();
    }

    void CanvasResource::put ( const FloatPoint& ptDst, unsigned char flags) const {
        putCanvas( ptDst, flags, *this, Screen::I()._impl->graphicMaterial );
    }

    void CanvasResource::put ( const Point& ptDst, unsigned char flags) const {
        putCanvas( ptDst, flags, *this, Screen::I()._impl->graphicMaterial );
    }


    unsigned int getPixel(unsigned int x, unsigned int y, RawImageResource raw) {
        if (!raw)
            throw Exception("Pixel data is not accesible for this configuration.");

        if ((int) x > raw->w || (int) y > raw->h)
            throw Exception("Wrong coordinates for getting a pixel from this surface.");

        unsigned int color = 0;
        int position = y * raw->pitch + raw->format->BytesPerPixel * x ;
        char* buffer = (char*) raw->pixels ;
        buffer += position ;
        memcpy ( &color , buffer , raw->format->BytesPerPixel ) ;
        return color;
    }


    // class BitmapGPU

    BitmapGPU::BitmapGPU(SDL_Surface* source)
        : _pixels(source), _tex(0), _w(0), _h(0)
    {}

    BitmapGPU::BitmapGPU(const void* pixels, GLuint w, GLuint h)
        : _pixels(nullptr), _tex(0), _w(pow2(w)), _h(pow2(h)), _relW(_w/w), _relH(_h/h)
    {
        load(pixels);
    }

    BitmapGPU::BitmapGPU()
        : _pixels(nullptr), _tex(0), _w(0), _h(0)
    {}

    BitmapGPU::~BitmapGPU() {
        destroyPixels();
        destroyTexture();
    }

    void BitmapGPU::save() const {
        if (isSoftware())
            return;

        if (saved())
            destroyPixels();

        assert(!_pixels);
        _pixels = new unsigned int[_w*_h];
        assert(_pixels);
        //TODO: glGetTexture
    }

    void BitmapGPU::unload() {
        destroyTexture();
    }

    void BitmapGPU::load(const void* pixels) {
        assert(pixels);
        assert(!_tex);

        GraphicFilter filter = NEAREST;

        // Have OpenGL generate a texture object handle for us
        glGenTextures(1, &_tex);
        if (!_tex)
            throw Exception("Texture can not be generated in storeTexture.\n"
                            "OpenGL Error: "+printGLErrors());

        // Bind the texture object
        glBindTexture( GL_TEXTURE_2D, _tex );

        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,filter == LINEAR ? GL_LINEAR : GL_NEAREST); //FIXME Provide more filters choices
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,filter == LINEAR ? GL_LINEAR : GL_NEAREST);
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    
        // http://www.opengl.org/wiki/GLAPI/glTexImage2D specify a two-dimensional texture image
        glTexImage2D( 
            GL_TEXTURE_2D,    /* GLenum target​        */
            0,                /* GLint level​​          */
            GL_RGBA,          /* GLint internalFormat​​ */
            _w,               /* GLsizei width​​        */
            _h,               /* GLsizei height​       */
            0,                /* GLint border​​         */
            GL_RGBA,          /* GLenum format​        */
            GL_UNSIGNED_BYTE, /* GLenum type​          */
            pixels            /* const GLvoid* data​   */
        );

        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    }

    void BitmapGPU::reload() {
        if (isSoftware())
            return;

        if (loaded()) {
            if (!saved())
                save();
            unload();
        } else if(!saved())
            throw Exception("No texture info for rebuilding bitmap");

        assert(_pixels);
        load(_pixels);
    }

    unsigned int BitmapGPU::getPixel(unsigned int x, unsigned int y) const {
        if (!loaded() && !saved())
            throw Exception("Impossible to get the pixel from a deleted texture");
        else if (!saved())
            save();

        auto pixels = isSoftware()? static_cast<SDL_Surface*>(_pixels)->pixels : _pixels;

        assert(pixels);

        unsigned int color    = 0, 
                     position = y * _w * 4 + x * 4;
        char* buffer = static_cast<char*>(_pixels);
        buffer += position ;
        memcpy ( &color , buffer , 4 ) ;
        return color;

    }

    void BitmapGPU::destroyPixels() const {
        if (saved())
            if (isSoftware())
                SDL_FreeSurface(static_cast<SDL_Surface*>(_pixels));
            else
                delete[] _pixels;

        _pixels = nullptr;
    }

    void BitmapGPU::destroyTexture() {
        if (loaded())
            glDeleteTextures( 1, &_tex );
        _tex = 0;
    }
} //FlatScene