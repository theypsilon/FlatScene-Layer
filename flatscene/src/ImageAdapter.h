#ifndef FS_IMAGE_ADAPTER__
#define FS_IMAGE_ADAPTER__

#include "Includes.h"

namespace FlatScene {

    typedef SDL_Surface*        RawImageResource;
    typedef const SDL_Surface&  ConstRawImageResource;

    typedef SDL_Rect            RectangleImage;

    class BitmapGPU {
    public:
        typedef GLuint            TexType;
        typedef GLuint           SizeType;
        typedef Float             RelType;
        typedef unsigned int    PixelType;

        BitmapGPU(const void* pixels, GLuint w, GLuint h);
        BitmapGPU(BitmapGPU&& that);
        BitmapGPU& operator=(BitmapGPU&& that);

        ~BitmapGPU();

        GLuint          getTex() const      { return _tex;               }
        GLuint          getW() const        { return _w;                 }
        GLuint          getH() const        { return _h;                 }
        Float           getRelW() const     { return _relW;              }
        Float           getRelH() const     { return _relH;              }

        bool            loaded() const      { return _tex != 0;          }
        bool            saved() const       { return _pixels != nullptr; }
        bool            isSoftware() const  { return false;              }

        void            save() const;
        void            unload();
        void            reload();

        unsigned int    getPixel(unsigned int x, unsigned int y) const;

    private:
        GLuint          _tex, _w, _h;
        Float           _relW, _relH;
        mutable void*   _pixels;

        void            load(const void* pixels);
        void            destroyPixels() const;
        void            destroyTexture();

        BitmapGPU(const BitmapGPU&);
        BitmapGPU& operator=(const BitmapGPU&);
    };
} // FlatScene;

#endif // FS_IMAGE_ADAPTER__