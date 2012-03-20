#ifndef __FS_CANVAS_IMPL__
#define __FS_CANVAS_IMPL__

#include "GL/gl.h"          // Librer�a OpenGL32
#include "GL/glu.h"
#include "SDL.h"
#include "SDL_image.h"

#include "FSCanvas.h"

namespace flatscene {

    struct Canvas::CanvasImpl {

        GLuint tex;
        Uint32 w, h;                /* Read-only */
        int w2,h2;          /* Valor previo desplazado a la potencia de 2 superior o igual m�s pr�xima. */
        Uint8 bpp;
        SDL_Surface* sdl_surf; // NULL or not null, thats the question.

        void clearSurface () {
            bpp = h = h2 = w = w2 = tex = 0;
            sdl_surf = nullptr;
        }

        mutable std::list<std::function<void(void)>> initCallbackList;
        mutable std::list<std::function<void(void)>> endCallbackList;

        ~CanvasImpl() {
            if (sdl_surf)
                SDL_FreeSurface(sdl_surf);

            if (h != 0 || w !=0)
                glDeleteTextures( 1, &tex );

            clearSurface();
        }


    };


} // flatscene

#endif
