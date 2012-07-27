#ifndef FS_RENDERS__
#define FS_RENDERS__

#include "CanvasResource.h"

namespace FlatScene {

    struct SRender {
        virtual void operator()()=0;
    };

    struct SRenderLocation : SRender { // LOCATE
        Float posx;
        Float posy;
        Float width;
        Float height;
        Float zoom;

        void operator()();
    };

    struct SRenderTranslation : SRender { // TRANSLATE && SCALATION
        Float x;
        Float y;
        Float z;

        void operator()();
    };

    struct SRenderScalation : SRender { // TRANSLATE && SCALATION
        Float x;
        Float y;
        Float z;

        void operator()();
    };

    struct SRenderRotation : SRender { // ROTATION
        Float angle;
        Float x;
        Float y;
        Float z;

        void operator()();
    };

    struct SRenderColor : SRender { // ROTATION
        Float red;
        Float green;
        Float blue;
        Float alpha;

        void operator()();
    };

    struct SRenderCanvas : SRender {
        Float relW, relH;
        int w2, h2;
        GLuint tex;
        Uint8 flags;

        SRenderCanvas(const CanvasResource& canvas, Uint8 flags)
        : relW((Float)(canvas.w2/canvas.w)), relH((Float)(canvas.h2/canvas.h)),
        w2(canvas.w2), h2(canvas.h2), tex(canvas.tex), flags(flags) {}

        void operator()();
    };


    struct SRenderPushMatrix : SRender { // ROTATION
        void operator()();
    };

    struct SRenderPopMatrix : SRender { // ROTATION
        void operator()();
    };

} // FlatScene

#endif // FS_RENDERS__