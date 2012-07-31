#include "CanvasResource.h"

namespace FlatScene {

    CanvasResource::~CanvasResource() {
        if (raw)
            SDL_FreeSurface(raw);

        if (h != 0 || w !=0)
            glDeleteTextures( 1, &tex );

        clearSurface();
    }

} //FlatScene