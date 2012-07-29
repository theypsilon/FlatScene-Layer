#ifndef FS_SCREEN_IMPL__
#define FS_SCREEN_IMPL__

#include "Screen.h"
#include "SpritesetResource.h"

namespace FlatScene {

typedef struct { 
    std::string name; 
    GraphicMode mode; 
    int times;
    int number;
} SpritesetInfo ;

typedef std::list<SpritesetInfo> GraphicResources;

struct SRender;

struct Screen::ScreenImpl {

    SDL_Surface* m_SDL_Surface;

    bool rendering;
    TypeRendeProjection trp;

    Float m_maxZ;

    bool m_FullScreen,m_Doublebuff;

    int m_Width,m_Height,m_Bpp;

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

};

} // flatscene

#endif
