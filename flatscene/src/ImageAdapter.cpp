#include "CanvasResource.h"
#include "Exception.h"
#include "Renders.h"
#include "ScreenImpl.h"
#include "sdlSurfaceFuncs.h"
#include <iostream>

namespace FlatScene {

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

    // class BitmapGPU

    BitmapGPU::BitmapGPU(BitmapGPU&& that) 
        : _pixels(that._pixels), _w(that._w), _h(that._h)
        , _tex(that._tex), _relW(that._relW), _relH(that._relH)
    {
        that._tex    = 0;
        that._pixels = nullptr;
    }

    BitmapGPU& BitmapGPU::operator=(BitmapGPU&& that) {
        this->_tex    = that._tex;
        this->_pixels = that._pixels;
        this->_w      = that._w;
        this->_h      = that._h;
        this->_relW   = that._relW;
        this->_relH   = that._relH;

        that._tex    = 0;
        that._pixels = nullptr;

        return *this;
    }

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

        glGetTexImage(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            _pixels
        );


        static int i = 0;
        if (i == 0) {
            for(;i< _w * _h;i++) {
                std::cout << static_cast<const unsigned int*>(_pixels)[i];
            }
            std::cout << std::endl << "end" << std::endl;
        }
    }

    void BitmapGPU::unload() {
        destroyTexture();
    }

    void BitmapGPU::load(const void* pixels) {
        assert(pixels);
        assert(!_tex);

        static int i = 0;
        if (i == 0) {
            for(;i< _w * _h;i++) {
                std::cout << static_cast<const unsigned int*>(pixels)[i];
            }
            std::cout << std::endl << "end" << std::endl;
        }

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