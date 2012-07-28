#include "ImagesPrivate.h"
#include "SpriteSetResource.h"
//#include "ScreenImpl.h"

#include <iostream>

namespace FlatScene {

void ImagesPrivate::clear() {

    bool needSave = nullptr != SDL_GetVideoSurface();
    for(auto& pair : count)
        if (needSave)
            ;//Screen::I()._impl->spritesetToDelete.push_back(std::move(it.first));
        else
            delete pair.first;

    count.clear();
}

SpritesetResource* ImagesPrivate::add(const std::string& name,unsigned char mode) {
    return new SpritesetResource(name,mode);
}

CanvasResource* ImagesPrivate::addSprite() {
    return new SpriteResource;
}

} // flatscene
