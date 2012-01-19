#include "FSLibrary.h"

#include "TestA0GameInterface.h"
#include "TestA1GameInterface.h"
#include "TestA2GameInterface.h"
#include "TestA3GameInterface.h"
#include "TestA4GameInterface.h"
#include "TestA5GameInterface.h"
#include "TestOSDesktopInterface.h"



void funcEstres(int p);



int main(int argc,char* argv[])
{
	//run the application
	FSLibrary::startLibrary(false);


	///*
	FSScreen::start(640,480,32,1.0,1.0,false);

	FSEngine** engine = new FSEngine*[20];
	
	engine[0] = new CTestA0GameInterface(&FSLibrary::getLibrary());

	engine[0]->setEventHandler(SDL_KEYDOWN,&CTestAGameInterface::onKeyTestA);
	engine[0]->setEventHandler(SDL_KEYUP,&CTestAGameInterface::onKeyTestA);
	FSLibrary::addEngine(engine[0],10);

	engine[1] = new CTestA3GameInterface(&FSLibrary::getLibrary());

	engine[1]->setEventHandler(SDL_KEYDOWN,&CTestAGameInterface::onKeyTestA);
	engine[1]->setEventHandler(SDL_KEYUP,&CTestAGameInterface::onKeyTestA);
	FSLibrary::addEngine(engine[1],11);

	engine[2] = new CTestA5GameInterface(&FSLibrary::getLibrary());

	engine[2]->setEventHandler(SDL_KEYDOWN,&CTestAGameInterface::onKeyTestA);
	engine[2]->setEventHandler(SDL_KEYUP,&CTestAGameInterface::onKeyTestA);
	FSLibrary::addEngine(engine[2],12);

	engine[3] = new CTestA4GameInterface(&FSLibrary::getLibrary());

	engine[3]->setEventHandler(SDL_KEYDOWN,&CTestAGameInterface::onKeyTestA);
	engine[3]->setEventHandler(SDL_KEYUP,&CTestAGameInterface::onKeyTestA);
	FSLibrary::addEngine(engine[3],13);

#ifdef LOG_SISTEMA
	printf("Se bienvenido, Persona.\n\n");
#endif

	Write.loadFont("resources/tahoma");

	FSLibrary::processEngines();

	delete engine;
	
	
	return(0);
}



void funcEstres(int p) {

	printf("\nlanzando rutinas de estres");



	for (int i = 0; i < p ; i++) {

		Img.remove(Img.add("resources/Character"));

		if (i> 300)
			FSScreen::render();

	}


	for (int i = 0; i < p ; i++) {

		Write.unloadFont(Write.loadFont("resources/tahoma"));

	}

	int f = Write.unloadFont(Write.loadFont("resources/tahoma"));

	for (int i = 0; i < p ; i++) {

		Write.erase(Write.line(f,i%100,0,"No se muestra"),true);

	}

	int s = Img.add("resources/Character");

	FSScreen::locateRenderScene();

	for (int i = 0; i < p ; i++) {

		FSPoint pt(i%100,rand()%200);

		Img.get(s)->get(0)->put(pt);

		FSScreen::render();

	}

	Img.remove(s);


	printf("\nprueba de estr�s finalizada");

}
