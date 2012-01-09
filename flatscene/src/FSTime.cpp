#include "FSTime.h"
#include "FSWriter.h"
#include "FSScreen.h"
#include "FSLibrary.h"


FSTime::FSTime() : actTime(NULL), admin(NULL), all(false), allMsInterval(16) {
#ifdef MENSAJES_FPS
		fps=0;
		auxTimer=0;
#endif
}

FSTime::~FSTime() {
}

int FSTime::getTick() {

	if (!FSLibrary::getLibrary()) {
		FSLibrary::Error("Library not inicialized");
		return FRACASO;
	}

	if (admin != FSLibrary::getActualEngine()) {
		admin = FSLibrary::getActualEngine();
		actTime = & fc[admin];
	}

	return actTime->frameCount;
}

int FSTime::setInterval(int msNew, bool all) {

	if (!all && admin != FSLibrary::getActualEngine()) {
		admin = FSLibrary::getActualEngine();
		actTime = & fc[admin];
	}

	int aux=0;

	if (all) {
		FSTime::all = true;
		aux = allMsInterval;
		allMsInterval = msNew;
	} else {
		FSTime::all = false;
		aux = actTime->msInterval;
		actTime->msInterval = msNew;
	}

	return aux;
}

int FSTime::setFPS(int fpsNew, bool all) {

	int aux = setInterval(1000 / fpsNew, all);

	if (aux == FRACASO)
		return FRACASO;

	return (1000 / aux);
}

int  FSTime::nextFrame() {

	int ret = EXITO;

	if (!FSLibrary::getLibrary()) {
		FSLibrary::Error("Library not inicialized");
		return FRACASO;
	}

	if (admin != FSLibrary::getActualEngine()) {
		admin = FSLibrary::getActualEngine();
		actTime = & fc[admin];
	}

	if ( FSScreen::render() == FRACASO )
		return FRACASO;

	if (all) {

		while ((actTime->msLast + allMsInterval) > SDL_GetTicks()) { 
			SDL_Delay(1); 
		}
		actTime->msLast = SDL_GetTicks();
		actTime->frameCount++;

	} else {

		while ((actTime->msLast + actTime->msInterval) > SDL_GetTicks()) { 
			SDL_Delay(1); 
		}
		actTime->msLast = SDL_GetTicks();
		actTime->frameCount++;

	}

#ifdef MENSAJES_FPS
	fps++;

	if (SDL_GetTicks() > auxTimer + 1000) {
		auxTimer=SDL_GetTicks();

		if (adminText.find(FSLibrary::getActualEngine())!=adminText.end())
			Write.erase(adminText[FSLibrary::getActualEngine()]);

		adminText[FSLibrary::getActualEngine()]=Write.line(0,5,5,"FPS: %d ",fps);

		fps=0;
	}
	
#endif

	return ret;
}

bool FSTime::isTimeForAll() {
	return all;
}

// TODO : Liberar los espacios de memoria correspondiente a los engines cuando estos sean eliminados. 
//				No es prioritario porque la creaci�n y destrucci�n de engines durante la ejecuci�n deber�a ser m�nima, y la memoria reservada es muy peque�a.

FSTime Chrono;
