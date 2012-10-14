#include "Exception.h"
#include "sdlSurfaceFuncs.h"
#include "ImageAdapter.h"
#include <iostream>

namespace FlatScene {

    // class BitmapGPU

    BitmapGPU::BitmapGPU(BitmapGPU&& that) 
        : _pixels(std::move(that._pixels)), _w(that._w), _h(that._h)
        , _tex(that._tex), _relW(that._relW), _relH(that._relH)
    {
        that._tex    = 0;
    }

    BitmapGPU& BitmapGPU::operator=(BitmapGPU&& rhs) {
        this->_pixels = std::move(rhs._pixels);

        this->_tex    = rhs._tex;
        this->_w      = rhs._w;
        this->_h      = rhs._h;
        this->_relW   = rhs._relW;
        this->_relH   = rhs._relH;

        rhs._tex    = 0;
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

        assert(_pixels.capacity() == 0);
        _pixels.resize(_w * _h);
        assert(_pixels.capacity() == _w * _h);

        glGetTexImage(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            _pixels.data()
        );
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

        assert(!_pixels.empty());
        load(_pixels.data());
    }

    unsigned int BitmapGPU::getPixel(unsigned int x, unsigned int y) const {
        if (!loaded() && !saved())
            throw Exception("Impossible to get the pixel from a deleted texture");
        else if (!saved())
            save();

        assert(!_pixels.empty());

        unsigned int color    = 0, 
                     position = y * _w * 4 + x * 4;
        memcpy ( &color , _pixels.data() + position , 4 ) ;
        return color;

    }

    void BitmapGPU::destroyPixels() const {
        if (saved())
            if (isSoftware())
                ;
            else  {
                _pixels.clear();
                _pixels.shrink_to_fit();
            }
    }

    void BitmapGPU::destroyTexture() {
        if (loaded())
            glDeleteTextures( 1, &_tex );
        _tex = 0;
    }
} //FlatScene