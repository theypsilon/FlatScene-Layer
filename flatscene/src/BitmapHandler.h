#ifndef FS_BITMAP_HANDLER__
#define FS_BITMAP_HANDLER__

#include "Includes.h"
#include "Types.h"
#include <cmath>

namespace FlatScene {

    namespace {
        inline unsigned int pow2(unsigned int n) {
            unsigned int c = 1;
            while (c < n) c <<= 1;
            return c;
        };

        inline std::vector<unsigned int> copyPixels(
            unsigned size, const unsigned*const pixels
        ) {
            std::vector<unsigned int> v;
            v.reserve(size);
            for (unsigned i = 0; i < size; i++)
                v[i] = pixels[i];
            return v;
        };
    } 

    class BitmapHandler {
    public:
        typedef GLuint                  TexType;
        typedef GLuint                  SizeType;
        typedef Float                   RelType;
        typedef unsigned int            PixelType;
        typedef std::vector<PixelType>  PAType;

        BitmapHandler(const bool software, const void*const pixels, SizeType w, SizeType h) 
        : _tex(0), _w(software? w : pow2(w)), _h(software? h : pow2(h))
        , _relW(software? 0 : _w/w), _relH(software? 0 : _h/h) {
            if (!software)
                copyExternalBufferToGPU(pixels);
            else
                _pixels = std::move(copyPixels(w*h, (unsigned*)pixels));
        }
        BitmapHandler(BitmapHandler&& that);
        BitmapHandler& operator=(BitmapHandler&& that);

        ~BitmapHandler();

        SizeType        getW() const    { return _relW > 0? (SizeType) std::ceil(_w / _relW) : _w; }
        SizeType        getH() const    { return _relH > 0? (SizeType) std::ceil(_h / _relH) : _h; }
        SizeType        getTexW() const      { return _w;                   }
        SizeType        getTexH() const      { return _h;                   }
        RelType         getRelW() const      { return _relW;                }
        RelType         getRelH() const      { return _relH;                }
        TexType         getGPUTex   () const { if (!inGPU()) copyToGPU(); return _tex;    }
        const PAType&   getCPUBuffer() const { if (!inCPU()) copyToCPU(); return _pixels; }

        bool            inGPU() const        { return _tex != 0;            }
        bool            inCPU() const        { return !_pixels.empty();     }
        bool            isSoftware() const   { return  inCPU() && !inGPU(); }
        bool            isHardware() const   { return !inCPU() &&  inGPU(); }
        bool            isBoth () const      { return  inCPU() &&  inGPU(); }
        bool            isEmpty() const      { return !inCPU() && !inGPU(); }
        bool            isSynchronized() const;

        void            setCPUTex(TexType t) { if (t) _tex = t;             }
        PAType&         setCPUBuffer()       { return _pixels;              }

        void            copyToCPU() const;
        void            copyToGPU() const;
        void            removeFromGPU();
        void            removeFromCPU();

        unsigned int    getPixel(unsigned int x, unsigned int y) const;

    private:
        mutable TexType  _tex;
        mutable SizeType _w, _h;
        mutable RelType  _relW, _relH;
        mutable PAType   _pixels;

        void            copyExternalBufferToGPU(const void*const pixels) const;
        void            clearCPUMemory() const;
        void            clearGPUMemory() const;

        BitmapHandler(const BitmapHandler&);
        BitmapHandler& operator=(const BitmapHandler&);
    };

}// FlatScene

#endif