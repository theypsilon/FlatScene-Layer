#include "IFirstEngine.h"

#include <exception>
#include <iostream>
#include <memory>
using namespace std;

int main (int argc, char* argv[]) {

    cout << "Starting main" << endl;

    FSLib.startLibrary(640,480,32,false);

    auto engine = unique_ptr<IFirstEngine>(new IFirstEngine);

    int e = Img.add("resources/tilp");

    auto spt = Img.get(e)->get(10);

    FSScreen& screen = FSScreen::I();

    for (;;) {
        screen.locateRenderScene(0,0,640,480);
        for (int i=0;i<100;i++)
            spt->put((10*(i%10),(i/10)*10));
        screen.render();
    }

    FSLib.processEngine(move(engine));

    cout << "Ending main" << endl;

    return 0;
}