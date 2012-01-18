#include "MenuAGameInterface.h"
#include "FSLibrary.h"
#include "FSControlImages.h"
#include "FSWriter.h"
#include "FSScreen.h"

//constructor
CMenuAGameInterface::CMenuAGameInterface(FSMessageHandler * pmhParent) : FSEngine(pmhParent)
{
	pushed=false;
	file=0;
	opcion=0;
	previous = NULL;
}

//destructor
CMenuAGameInterface::~CMenuAGameInterface()
{

}

//initialization
int CMenuAGameInterface::onInit() {

	if (FSEngine::onInit() == FRACASO)
		return FRACASO;

#ifdef LOG_SISTEMA
	printf("\nMenuA comienza.\n\n");
#endif

	file=Img.add("resources/Menu");

	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,60);

	dest.set(22,25);

	FSScreen::locateRenderScene();

	texts.push_back("Continuar");
	texts.push_back("Modo de Pantalla : ");
	texts.push_back("FPS : ");
	texts.push_back("Reiniciar");
	texts.push_back("Salir");

	int y = 40;
	for (list<string>::iterator it=texts.begin();it!=texts.end();++it) {
		IdTexts.push_back(Write.line(0,60,y,"%s",it->c_str()));
		y+=30;
	}

	char c[32];
	if (!FSScreen::isFullscreen())
		sprintf(c,"Modo Ventana");
	else
		sprintf(c,"Pantalla Completa");

	IdTexts.push_back(Write.line(0,232,70,"%s",c));

	int aux = Chrono.setInterval(0,Chrono.isTimeForAll());
	if (aux == 0) {
		sprintf(c,"ilimitado");
	} else {
		sprintf(c,"%d",1000/aux);
		Chrono.setInterval(aux,Chrono.isTimeForAll());
	}

	IdTexts.push_back(Write.line(0,110,100,"%s",c));

	return EXITO;
}

//idle. Main loop.
int CMenuAGameInterface::onIdle()
{
	FSScreen::clear();

	FSScreen::locateRenderScene(0,0,RESOLUCION_X*2,RESOLUCION_Y*2);

	FSPoint m = dest;

	FSScreen::pushMatrix();

	FSScreen::scale(2.0,2.0,1.0);

	Img.get(file)->get(0)->put(m);

	FSScreen::popMatrix();

	Write.render();
	
	return EXITO;
}

//on cleanup
int CMenuAGameInterface::onExit()
{

	SDL_EnableKeyRepeat(0,0);

	Img.remove(file);
#ifdef LOG_SISTEMA
	printf("\nMenuA termina.\n\n");
#endif

	return FSEngine::onExit();

}

FSEngine* CMenuAGameInterface::setPrevious(FSEngine* newE) {
	FSEngine* ret = previous;
	previous = newE;
	return ret;
}

void CMenuAGameInterface::onKeyMenu(SDL_Event* event) {
	if (event->type == SDL_KEYDOWN)
		onKeyDown(event->key.keysym.sym,event->key.keysym.mod,event->key.keysym.unicode);
	else
		onKeyUp(event->key.keysym.sym,event->key.keysym.mod,event->key.keysym.unicode);
}


void CMenuAGameInterface::onKeyDown(SDLKey sym,SDLMod mod,Uint16 unicode) {
	if ((sym==SDLK_ESCAPE || sym==SDLK_RETURN)&& !pushed) {
		pushed=true;
	} else if (sym == SDLK_UP) {
		if (opcion == 0)
			opcion=texts.size()-1;
		else
			opcion--;

	} else if (sym == SDLK_DOWN) {
		if (opcion == (texts.size()-1))
			opcion=0;
		else
			opcion++;
	}

	dest.y=25+15*opcion;
}

void CMenuAGameInterface::onKeyUp(SDLKey sym,SDLMod mod,Uint16 unicode) {
	if (sym==SDLK_ESCAPE && pushed) {
		getParent()->SendMessage(FSLibrary::MSGID_RunEngine,(void*)previous);
		getParent()->SendMessage(FSLibrary::MSGID_KillEngine,(void*)this);
	} else if (sym==SDLK_RETURN && pushed) {
		if (opcion == 0) {
			getParent()->SendMessage(FSLibrary::MSGID_RunEngine,(void*)previous);
			getParent()->SendMessage(FSLibrary::MSGID_KillEngine,(void*)this);
		}	else if (opcion == 1) {
			FSScreen::ToggleFullscreen();

			char c[32];
			if (!FSScreen::isFullscreen())
				sprintf(c,"Modo Ventana");
			else
				sprintf(c,"Pantalla Completa");
			try {
				int& y = IdTexts.at(texts.size()+0);
				Write.erase(y);
				y=Write.line(0,232,70,"%s",c);
			} catch (...) {
				FSLibrary::Error("MenuA IdTexts vector bad access");
			}
		
		} else if (opcion == 2) {
			int aux = Chrono.setInterval(0,true);
			if (aux == 0)	 {
				aux=16;
				FSScreen::setDoublebuffer(true);
			} else {
				aux=0;
				FSScreen::setDoublebuffer(false);
			}

			char c[32];
			if (aux == 0) {
				sprintf(c,"ilimitado");
			} else {
				sprintf(c,"%d",1000/aux);

				if (Chrono.isTimeForAll())
					aux = Chrono.setInterval(aux);

				Chrono.setInterval(aux);
			}

			try {
				int& y = IdTexts.at(texts.size()+1);
				Write.erase(y);
				y=Write.line(0,110,100,"%s",c);
			} catch (...) {
				FSLibrary::Error("MenuA IdTexts vector bad access");
			}
		} else if (opcion == 3) {
			getParent()->SendMessage(FSLibrary::MSGID_KillEngine,(void*)this);
			getParent()->SendMessage(FSLibrary::MSGID_Restart);
			
		} else if (opcion == 4) {
			getParent()->SendMessage(FSLibrary::MSGID_KillEngine,(void*)this);
			getParent()->SendMessage(FSLibrary::MSGID_Exit);
		}
		
		pushed=false;
	}
}
