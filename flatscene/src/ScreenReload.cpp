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

void ScreenImpl::goOnAllGPUs(std::function<void(BitmapHandler&)> process) {
    for (auto&& wpCanvas : gCRMonitor)
        process(wpCanvas->_gpu);
}

void ScreenImpl::saveResources() {
    goOnAllGPUs([](BitmapHandler& gpu){
        gpu.copyToCPU();
        gpu.removeFromGPU();
    });
}

void ScreenImpl::reloadResources() {
    goOnAllGPUs([](BitmapHandler& gpu){
        gpu.copyToGPU();  
    });
}

} // flatscene
