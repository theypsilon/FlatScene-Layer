#ifndef __FS_IMAGES_IMPL__
#define __FS_IMAGES_IMPL__

#include "FSScreen.h"

struct FSScreen::ScreenImpl {
    SDL_Surface* m_SDL_Surface;

    bool rendering;
    TypeRendeProjection trp;

    float m_maxZ;

    bool m_FullScreen,m_Doublebuff;

    int m_Width,m_Height,m_Bpp;

    list<SToRender*> graphicMaterial;

    list<FSSprite*> spriteToDelete;
    list<FSSpriteset*> spritesetToDelete;
    list<FSCanvas*> imageToDelete;

    void deleteResources();

    void saveResources(GraphicResources& info);
    void reloadResources(GraphicResources& info);


    float red,green,blue,alpha;


    map<TypeResource,void (*)(void*)> procRenders;

    void initProcRenders();

    static void procRendCanvas(void* pointer);
    static void procRendFloatCanvas(void* pointer);
    static void procRendRotation(void* pointer);
    static void procRendTranslation(void* pointer);
    static void procRendLocation(void* pointer);
    static void procRendPush(void* pointer);
    static void procRendPop(void* pointer);
    static void procRendScalation(void* pointer);
    static void procRendColor(void* pointer);

    int beginRenderMode(Uint32 flags);
    int endRenderMode(Uint32 flags);
};

#endif
