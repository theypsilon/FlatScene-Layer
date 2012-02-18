#include "FSLibrary.h"

#include "TestA0GameInterface.h"
#include "TestA1GameInterface.h"
#include "TestA2GameInterface.h"
#include "TestA3GameInterface.h"
#include "TestA4GameInterface.h"
#include "TestA5GameInterface.h"
#include "TestOSDesktopInterface.h"

#include <memory>

void funcEstres(int p);

using namespace std;

int main(int argc,char* argv[])
{
    //run the application
    FSLib.startLibrary(false);


    ///*
    FSDraw.start(640,480,32,1.0,1.0,false);

    unique_ptr<FSEngine> engine = nullptr;
    
    engine = unique_ptr<FSEngine>(new CTestA0GameInterface);

    engine->setEventHandler(SDL_KEYDOWN,&CTestAGameInterface::onKeyTestA);
    engine->setEventHandler(SDL_KEYUP,&CTestAGameInterface::onKeyTestA);
    FSLib.addEngine(move(engine),10);

    engine =  unique_ptr<FSEngine>(new CTestA3GameInterface);

    engine->setEventHandler(SDL_KEYDOWN,&CTestAGameInterface::onKeyTestA);
    engine->setEventHandler(SDL_KEYUP,&CTestAGameInterface::onKeyTestA);
    FSLib.addEngine(move(engine),11);

    engine =  unique_ptr<FSEngine>(new CTestA5GameInterface);

    engine->setEventHandler(SDL_KEYDOWN,&CTestAGameInterface::onKeyTestA);
    engine->setEventHandler(SDL_KEYUP,&CTestAGameInterface::onKeyTestA);
    FSLib.addEngine(move(engine),12);

    engine =  unique_ptr<FSEngine>(new CTestA4GameInterface);

    engine->setEventHandler(SDL_KEYDOWN,&CTestAGameInterface::onKeyTestA);
    engine->setEventHandler(SDL_KEYUP,&CTestAGameInterface::onKeyTestA);
    FSLib.addEngine(move(engine),13);

#ifdef LOG_SISTEMA
    printf("Se bienvenido, Persona.\n\n");
#endif

    Write.loadFont("resources/tahoma");

    FSLib.processEngines();
    
    
    return(0);
}



void funcEstres(int p) {

    printf("\nlanzando rutinas de estres");



    for (int i = 0; i < p ; i++) {

        Img.remove(Img.add("resources/Character"));

        if (i> 300)
            FSDraw.render();

    }


    for (int i = 0; i < p ; i++) {

        Write.unloadFont(Write.loadFont("resources/tahoma"));

    }

    int f = Write.unloadFont(Write.loadFont("resources/tahoma"));

    for (int i = 0; i < p ; i++) {

        Write.erase(Write.line(f,i%100,0,"No se muestra"),true);

    }

    int s = Img.add("resources/Character");

    FSDraw.locateRenderScene();

    for (int i = 0; i < p ; i++) {

        FSPoint pt(i%100,rand()%200);

        Img.get(s)->get(0)->put(pt);

        FSDraw.render();

    }

    Img.remove(s);


    printf("\nprueba de estr�s finalizada");

}
