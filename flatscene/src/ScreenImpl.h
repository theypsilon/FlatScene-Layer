#ifndef FS_SCREEN_IMPL__
#define FS_SCREEN_IMPL__

#include "Includes.h"
#include "Screen.h"
#include "ImageSetResource.h"

namespace FlatScene {

struct SRender;

struct ScreenImpl {

    SDL_Surface*            m_SDL_Surface;

    bool                    rendering;
    TypeRendeProjection     trp;

    Float                   m_maxZ;

    bool                    m_FullScreen, m_Doublebuff;

    int                     m_Width, m_Height,m_Bpp;

    std::list<SRender  *>   graphicMaterial;

    std::list<Sprite   *>   spriteToDelete;
    std::list<Spriteset*>   spritesetToDelete;
    std::list<Canvas   *>   imageToDelete;

    Float                   red, green, blue, alpha;

    void                    deleteResources();

    void                    goOnAllGPUs(std::function<void(BitmapHandler&)> process);
    void                    saveResources  ();
    void                    reloadResources();

    int                     beginRenderMode(unsigned flags);
    int                     endRenderMode  (unsigned flags);

    friend class Screen;
};

} // flatscene

#endif
