#ifndef __FS_SCREEN_IMPL__
#define __FS_SCREEN_IMPL__

#include "FSScreen.h"

struct FSScreen::ScreenImpl {

    SDL_Surface* m_SDL_Surface;

    bool rendering;
    TypeRendeProjection trp;

    float m_maxZ;

    bool m_FullScreen,m_Doublebuff;

    int m_Width,m_Height,m_Bpp;

    list<SRender*> graphicMaterial;

    list<FSSprite*> spriteToDelete;
    list<FSSpriteset*> spritesetToDelete;
    list<FSCanvas*> imageToDelete;

    void deleteResources();

    void saveResources(GraphicResources& info);
    void reloadResources(GraphicResources& info);


    float red,green,blue,alpha;

    int beginRenderMode(Uint32 flags);
    int endRenderMode(Uint32 flags);

};

#endif
