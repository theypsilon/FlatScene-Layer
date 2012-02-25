#include "IFirstEngine.h"

#include <exception>
#include <iostream>
#include <memory>
using namespace std;

int main (int argc, char* argv[]) {

    cout << "Starting main" << endl;

    //run the application
    FSLib.startLibrary(false);
    FSDraw.start(640,480,32,1.0,1.0,false);
    Write.loadFont("resources/tahoma");
    FSPoint pt(0,0);

    auto surf = IMG_Load("resources/tilp.png");

    int index = Img.add("resources/tilp",WITH_SDL_SURFACE);
    auto& spt = *Img.get(index)->get(0);
    FSSprite chipset(FSCanvas::toSCanvas(surf,WITH_SDL_SURFACE),pt);
    for (int timer = SDL_GetTicks() ; timer + 4000 > SDL_GetTicks();) {
            FSDraw.projectionMode(TRP_PERSPECTIVE,1600);
            FSDraw.locateRenderScene(0,0,640,480);
            pt.set(0,0);
            chipset.put(pt);
            for (int i = 0;i < 100; i++) {
                pt.set((i%20)*32,320+(i/20)*32);
                spt.put(pt,0);
            }
            FSDraw.render();
            SDL_Delay(16);
    }
    
    return(0);

    cout << "Ending main" << endl;

    return 0;
}