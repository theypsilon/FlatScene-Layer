#ifndef FS_SCREEN_IMPL__
#define FS_SCREEN_IMPL__

#include "Screen.h"
#include "CanvasImpl.h"

namespace FlatScene {

typedef struct { 
    std::string name; 
    Uint8 mode; 
    int times;
    int number;
} SpritesetInfo ;

typedef std::list<SpritesetInfo> GraphicResources;

struct Screen::ScreenImpl {

    SDL_Surface* m_SDL_Surface;

    bool rendering;
    TypeRendeProjection trp;

    Float m_maxZ;

    bool m_FullScreen,m_Doublebuff;

    int m_Width,m_Height,m_Bpp;

    struct SRender {
        virtual void operator()()=0;
    };

    std::list<SRender*> graphicMaterial;

    std::list<Sprite*> spriteToDelete;
    std::list<Spriteset*> spritesetToDelete;
    std::list<Canvas*> imageToDelete;

    void deleteResources();

    void saveResources(GraphicResources& info);
    void reloadResources(GraphicResources& info);


    Float red,green,blue,alpha;

    int beginRenderMode(Uint32 flags);
    int endRenderMode(Uint32 flags);

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

};

} // flatscene

#endif
