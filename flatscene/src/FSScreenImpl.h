#ifndef __FS_SCREEN_IMPL__
#define __FS_SCREEN_IMPL__

#include "FSScreen.h"

typedef struct { 
    std::string name; 
    Uint8 mode; 
    int times;
    int number;
} SpritesetInfo ;

typedef std::list<SpritesetInfo> GraphicResources;

struct FSScreen::ScreenImpl {

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

    std::list<FSSprite*> spriteToDelete;
    std::list<FSSpriteset*> spritesetToDelete;
    std::list<FSCanvas*> imageToDelete;

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

    template <class PointType>
    struct SRenderCanvas : SRender {
        SCanvas canvas;
        PointType ptDst;
        Uint8 flags;

        SRenderCanvas<PointType>(SCanvas canvas, PointType ptDst, Uint8 flags)
        : canvas(canvas), ptDst(ptDst), flags(flags) {}

        void operator()() {
            if (canvas.h != 0 || canvas.w !=0 ) {

                glBindTexture(GL_TEXTURE_2D, canvas.tex);

                Float relW = (Float)canvas.w2/(Float)canvas.w;
                Float relH = (Float)canvas.h2/(Float)canvas.h;

                //glScalef((1.0/m_ScaleX ),(1.0/m_ScaleY ),0.0);

                glBegin(GL_QUADS);
                    if (flags == 0) {
                        glTexCoord2f(0.0f, relH);
                        glVertex2f(0, canvas.h2);
                        glTexCoord2f(relW, relH);
                        glVertex2f(canvas.w2, canvas.h2);
                        glTexCoord2f(relW, 0.0f);
                        glVertex2f(canvas.w2, 0);
                        glTexCoord2f(0.0f, 0.0f);
                        glVertex2f(0,0);
                    } else if (flags == 1) {

                        glTexCoord2f(relW, relH);
                        glVertex2f(0, canvas.h2);
                        glTexCoord2f(0.0f, relH);
                        glVertex2f(canvas.w2, canvas.h2);
                        glTexCoord2f(0.0f, 0.0f);
                        glVertex2f(canvas.w2, 0);
                        glTexCoord2f(relW, 0.0f);
                        glVertex2f(0,0);
                    } else if (flags==2) {
                        glTexCoord2f(0.0f, 0.0f);
                        glVertex2f(0, canvas.h2);
                        glTexCoord2f(relW, 0.0f);
                        glVertex2f(canvas.w2, canvas.h2);
                        glTexCoord2f(relW, relH);
                        glVertex2f(canvas.w2, 0);
                        glTexCoord2f(0.0f, relH);
                        glVertex2f(0,0);
                    } else  {
                        glTexCoord2f(relW, 0.0f);
                        glVertex2f(0, canvas.h2);
                        glTexCoord2f(0.0f, 0.0f);
                        glVertex2f(canvas.w2, canvas.h2);
                        glTexCoord2f(0.0f, relH);
                        glVertex2f(canvas.w2, 0);
                        glTexCoord2f(relW, relH);
                        glVertex2f(0,0);
                    }
                glEnd();

            }
            delete this;
        };
    };


    struct SRenderPushMatrix : SRender { // ROTATION
        void operator()();
    };

    struct SRenderPopMatrix : SRender { // ROTATION
        void operator()();
    };

};

#endif
