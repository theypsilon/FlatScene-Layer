#include "DebugLibrary.h"

namespace FlatScene { namespace Debug {

    //GLuint* i = nullptr;
    //CanvasResource* canvas = nullptr;

    void doDebug0() {
/*
        for (auto pair : RefCountMemoryPolicy<CanvasResource>::getCounts())
            if (pair.first->tex == 1) {
                canvas = const_cast<CanvasResource*>(pair.first);
                i = &canvas->tex;
            }

        if (canvas && i) {
            std::cout << "Tex " << *i << " going deleted" << std::endl;
            canvas->cleanResourcesGPU();
            std::cout << "Tex " << *i << " after deleted" << std::endl;
        }*/
    }

    void doDebug1() {
/*        if (canvas && i) {
            std::cout << "Tex " << *i << " going reloaded" << std::endl;
            reloadSurface(*canvas);
            std::cout << "Tex " << *i << " after reloaded" << std::endl;
        }*/
    }

}} // FlatScene::Debug