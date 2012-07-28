#ifndef FS_CANVAS_RES__
#define FS_CANVAS_RES__

#include "Canvas.h"
#include "Point.h"
#include "RefCountMemoryPolicyImpl.h"

namespace FlatScene {

    class CanvasResource {
    public:
        typedef Point PointType;

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


    };


} // flatscene

#endif
