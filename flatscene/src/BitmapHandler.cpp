#include "Exception.h"
#include "sdlSurfaceFuncs.h"
#include "BitmapHandler.h"
#include <iostream>

namespace FlatScene {

    // class BitmapHandler with OpenGL

    namespace detail {

        void glFillVectorFromTexture(std::vector<unsigned int>& v,unsigned int tex) {
            glBindTexture( GL_TEXTURE_2D, tex );
            glGetTexImage(
                GL_TEXTURE_2D,
                0,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                v.data()
            );
        }

    } //detail

    BitmapHandler::BitmapHandler(BitmapHandler&& that) 
        : _pixels(std::move(that._pixels)), _w(that._w), _h(that._h)
        , _tex(that._tex), _relW(that._relW), _relH(that._relH)
    {
        that._tex    = 0;
    }

    BitmapHandler& BitmapHandler::operator=(BitmapHandler&& rhs) {
        this->_pixels = std::move(rhs._pixels);

        this->_tex    = rhs._tex;
        this->_w      = rhs._w;
        this->_h      = rhs._h;
        this->_relW   = rhs._relW;
        this->_relH   = rhs._relH;

        rhs._tex    = 0;
        return *this;
    }

    BitmapHandler::~BitmapHandler() {
        clearCPUMemory();
        clearGPUMemory();
    }

    void BitmapHandler::load(const void*const pixels) {
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

    bool BitmapHandler::isSynchronized() const {
        if (!isBoth())
            return false;

        std::vector<unsigned int> v(_pixels.size());
        detail::glFillVectorFromTexture(v,_tex);
        return std::equal(begin(v),end(v),begin(_pixels));
    }

    void BitmapHandler::copyToCPU() const {
        if (isSoftware())
            return;

        if (inCPU())
            clearCPUMemory();

        assert(_pixels.capacity() == 0);
        _pixels.resize(_w * _h);
        assert(_pixels.capacity() == _w * _h);
        assert(_tex);

        detail::glFillVectorFromTexture(_pixels,_tex);
        assert(inCPU());
    }

    void BitmapHandler::copyToGPU() {
        if (isHardware())
            return;

        if (isSoftware()) {
            unsigned int
            tempw = _w,
            temph = _h;
            _relW =(_w = detail::pow2(_w));
            _relH =(_h = detail::pow2(_h));
            _relW/= tempw;
            _relH/= temph;
        } else if (inGPU()) { /* true for isBoth */
            removeFromGPU();
        } else throw Exception("No texture info saved for rebuilding bitmap");

        assert(inCPU());
        load(_pixels.data());
        assert(inGPU());
    }

    void BitmapHandler::removeFromGPU() {
        if (isHardware()) throw Exception("makes no sense to let a BitmapHandler with an empty state");

        clearGPUMemory();

        assert(inCPU());
        _w = std::round((_w / _relW)+0.5);
        _h = std::round((_h / _relH)+0.5);
        _relW = _relH = 0;
        assert(isSoftware());
    }

    void BitmapHandler::removeFromCPU() {
        if (isSoftware()) throw Exception("makes no sense to let a BitmapHandler with an empty state");

        clearCPUMemory();

        assert(isHardware());
    }

    void BitmapHandler::clearCPUMemory() const {
        if (inCPU()) {
            _pixels.clear();
            _pixels.shrink_to_fit();
        }
    }

    void BitmapHandler::clearGPUMemory() {
        if (inGPU())
            glDeleteTextures( 1, &_tex );
        _tex = 0;
    }

    unsigned int BitmapHandler::getPixel(unsigned int x, unsigned int y) const {
        assert(!isEmpty());
        
        if (!inCPU())
            copyToCPU();

        assert(inCPU());
        return _pixels[y*_w + x];
    }

} //FlatScene