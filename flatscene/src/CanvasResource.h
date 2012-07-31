#ifndef FS_CANVAS_RES__
#define FS_CANVAS_RES__

#include "Canvas.h"
#include "Point.h"
#include "RefCountMemoryPolicyImpl.h"

namespace FlatScene {

    class CanvasResource {
        typedef Canvas          Handler;
        typedef Point           PointType;

        CanvasResource (const PointType& nxy, const SDL_Surface *const nc) : xy(nxy), c(nc) {}

        GLuint                      tex;
        Uint32                      w, h;                /* Read-only */
        int                         w2,h2;          /* Valor previo desplazado a la potencia de 2 superior o igual m�s pr�xima. */
        Uint8                       bpp;
        SDL_Surface*                sdl_surf; // NULL or not null, thats the question.
        const PointType             xy;
        const SDL_Surface* const    c;

        void clearSurface () {
            bpp = h = h2 = w = w2 = tex = 0;
            sdl_surf = nullptr;
        }

        mutable std::list<std::function<void(void)>> initCallbackList;
        mutable std::list<std::function<void(void)>> endCallbackList;

        virtual ~CanvasResource() {
            if (sdl_surf)
                SDL_FreeSurface(sdl_surf);

            if (h != 0 || w !=0)
                glDeleteTextures( 1, &tex );

            clearSurface();
        }

        friend class Canvas;
        friend class SRenderCanvas;
        friend class RefCountMemoryPolicy<CanvasResource>;
        friend class SpriteResource;

        template <class T> friend T* createResource(
            const SDL_Rect& src, const SDL_Surface& chipset, 
            GraphicMode mode, double sp_scale, GraphicFilter filter = NEAREST
        );

        template <typename PointType, typename GraphicMaterial>
        friend void putCanvas ( const PointType& ptDst, unsigned char flags, 
                                const CanvasResource& impl, GraphicMaterial& gm );

        friend void storeSurface(
            CanvasResource& canvas, SDL_Surface* surface, GraphicMode mode, GraphicFilter filter
        );
    };


} // flatscene

#endif
