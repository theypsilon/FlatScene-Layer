#include "Library.h"
#include "ScreenImpl.h"
#include "ImageSet.h"
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
        auto& gpu = fCanvas.first->_gpu;

        gpu.copyToCPU();
        gpu.removeFromGPU();
    }

}

void ScreenImpl::reloadResources(GraphicResources &info) {
    for (auto& fCanvas : RefCountMemoryPolicy<CanvasResource>::getCounts()) {
        auto& gpu = fCanvas.first->_gpu;
        
        gpu.copyToGPU();     
    }
}

} // flatscene
