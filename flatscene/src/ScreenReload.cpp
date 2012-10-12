#include "Library.h"
#include "ScreenImpl.h"
#include "SpriteSet.h"
#include "sdlSurfaceFuncs.h"

namespace FlatScene {

void ScreenImpl::deleteResources() {
    
    for (auto it = spritesetToDelete.begin(), jt = spritesetToDelete.end() ; it!=jt;++it)
        delete (*it);

    spritesetToDelete.clear();
    
    for (auto it = spriteToDelete.begin(), jt = spriteToDelete.end() ; it!=jt;++it)
        delete (*it);

    spriteToDelete.clear();

    for (auto it = imageToDelete.begin(), jt = imageToDelete.end() ; it!=jt;++it)
        delete (*it);

    imageToDelete.clear();
}

void ScreenImpl::saveResources(GraphicResources &info) {

    for (auto& fCanvas : RefCountMemoryPolicy<CanvasResource>::getCounts()) {
        fCanvas.first->_gpu.save();
        fCanvas.first->_gpu.unload();
    }

}

void ScreenImpl::reloadResources(GraphicResources &info) {
    for (auto& fCanvas : RefCountMemoryPolicy<CanvasResource>::getCounts()) {
        fCanvas.first->_gpu.reload();
    }
}

} // flatscene
