#ifndef FS_IMAGE_ADAPTER__
#define FS_IMAGE_ADAPTER__

#include "Includes.h"

namespace FlatScene {

    typedef SDL_Surface*        RawImageResource;
    typedef const SDL_Surface&  ConstRawImageResource;

    typedef SDL_Rect            RectangleImage;

    unsigned int getPixel(unsigned int x, unsigned int y, RawImageResource raw);

    class BitmapGPU {
    public:
        typedef std::function<unsigned int(unsigned int, unsigned int)> PixelGetter;

        BitmapGPU(const SDL_Surface* source);
        BitmapGPU();
        ~BitmapGPU();

        GLuint          getTex() const      { return _tex;               }
        GLuint          getW() const        { return _w;                 }
        GLuint          getH() const        { return _h;                 }

        bool            loaded() const      { return _tex != 0;          }
        bool            saved() const       { return _pixels != nullptr; }

        void            save() const;
        void            unload();
        void            reload();

        PixelGetter     getPixelGetter() const;

    private:
        GLuint          _tex, _w, _h;
        mutable void*   _pixels;

        void            load(const void* pixels);
        void            destroyPixels() const;
        void            destroyTexture();
    };
} // FlatScene;

#endif // FS_IMAGE_ADAPTER__