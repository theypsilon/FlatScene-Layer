#include "FSTime.h"
#include "FSControlOutputText.h"
#include "FSScreen.h"
#include "FSLibrary.h"


CTime::CTime() : actTime(NULL), admin(NULL), all(false), allMsInterval(16) {
#ifdef MENSAJES_FPS
		fps=0;
		auxTimer=0;
#endif
}

CTime::~CTime() {
}

int CTime::getTick() {

	if (!CLibrary::getLibrary()) {
		CLibrary::Error("Library not inicialized");
		return FRACASO;
	}

	if (admin != CLibrary::getActualEngine()) {
		admin = CLibrary::getActualEngine();
		actTime = & fc[admin];
	}

	return actTime->frameCount;
}

int CTime::setInterval(int msNew, bool all) {

	if (!all && admin != CLibrary::getActualEngine()) {
		admin = CLibrary::getActualEngine();
		actTime = & fc[admin];
	}

	int aux=0;

	if (all) {
		CTime::all = true;
		aux = allMsInterval;
		allMsInterval = msNew;
	} else {
		CTime::all = false;
		aux = actTime->msInterval;
		actTime->msInterval = msNew;
	}

	return aux;
}

int CTime::setFPS(int fpsNew, bool all) {

	int aux = setInterval(1000 / fpsNew, all);

	if (aux == FRACASO)
		return FRACASO;

	return (1000 / aux);
}

int  CTime::nextFrame() {

	int ret = EXITO;

	if (!CLibrary::getLibrary()) {
		CLibrary::Error("Library not inicialized");
		return FRACASO;
	}

	if (admin != CLibrary::getActualEngine()) {
		admin = CLibrary::getActualEngine();
		actTime = & fc[admin];
	}

	if ( CScreen::render() == FRACASO )
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

		if (adminText.find(CLibrary::getActualEngine())!=adminText.end())
			Write.erase(adminText[CLibrary::getActualEngine()]);

		adminText[CLibrary::getActualEngine()]=Write.line(0,5,5,"FPS: %d ",fps);

		fps=0;
	}
	
#endif

	return ret;
}

bool CTime::isTimeForAll() {
	return all;
}

// TODO : Liberar los espacios de memoria correspondiente a los engines cuando estos sean eliminados. 
//				No es prioritario porque la creación y destrucción de engines durante la ejecución debería ser mínima, y la memoria reservada es muy pequeña.

CTime Chrono;
