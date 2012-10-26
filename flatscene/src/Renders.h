#ifndef FS_RENDERS__
#define FS_RENDERS__

#include "Types.h"

namespace FlatScene {

    class BitmapHandler;

    struct SRender {
        virtual void operator()()=0;
    };

    struct SRenderLocation : SRender { // LOCATE
        Float posx;
        Float posy;
        Float width;
        Float height;
        Float zoom;

        virtual void operator()();
    };

    struct SRenderTranslation : SRender { // TRANSLATE && SCALATION
        Float x;
        Float y;
        Float z;

        virtual void operator()();
    };

    struct SRenderScalation : SRender { // TRANSLATE && SCALATION
        Float x;
        Float y;
        Float z;

        virtual void operator()();
    };

    struct SRenderRotation : SRender { // ROTATION
        Float angle;
        Float x;
        Float y;
        Float z;

        virtual void operator()();
    };

    struct SRenderColor : SRender { // ROTATION
        Float red;
        Float green;
        Float blue;
        Float alpha;

        virtual void operator()();
    };

    struct SRenderCanvas : SRender {
        const BitmapHandler& bm;
        unsigned char flags;

        SRenderCanvas(const BitmapHandler& bm, unsigned char flags)
        : bm(bm), flags(flags) {}

        virtual void operator()();
    };


    struct SRenderPushMatrix : SRender { // ROTATION
        virtual void operator()();
    };

    struct SRenderPopMatrix : SRender { // ROTATION
        virtual void operator()();
    };

} // FlatScene

#endif // FS_RENDERS__