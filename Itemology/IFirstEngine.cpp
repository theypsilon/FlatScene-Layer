#include "IFirstEngine.h"
#include "GenericAlgorithms.h"

IFirstEngine::IFirstEngine(CMessageHandler* pmhParent) : CEngine(pmhParent)	{

	move[i_up] = false;
	move[i_down] = false;
	move[i_left] = false;
	move[i_right] = false;

	setEventHandler(SDL_KEYDOWN,&IFirstEngine::onKeyboard);
	setEventHandler(SDL_KEYUP,&IFirstEngine::onKeyboard);

}

IFirstEngine::~IFirstEngine()	{

	// TODO
}

void IFirstEngine::onKeyboard(SDL_Event* e) {

	bool turn = true;

	if (e->type == SDL_KEYUP)
		turn = false;

	if ( e->key.keysym.sym == SDLK_LEFT )
		move[i_left] = turn;

	if ( e->key.keysym.sym == SDLK_DOWN )
		move[i_down] = turn;

	if ( e->key.keysym.sym == SDLK_UP )
		move[i_up] = turn;

	if ( e->key.keysym.sym == SDLK_RIGHT )
		move[i_right] = turn;

}

int IFirstEngine::drawFrame() {

	// TODO : Fase de renderización del Motor.

	// Ejemplo de renderización simple suponiendo que disponemos de un Conjunto de Cámaras

	for (auto i = cams.begin(); i != cams.end(); ++i) {
		(*i)->render();
	}

	Write.render();

	return EXITO;
}

int  IFirstEngine::onInit() {

	CEngine::onInit();


	IScrollLevel* level = new IScrollLevel("resources/mapa1");

	CMultiverse.add(level);

	level->load();

	mainactor = new IScrollObject(this);

	mainactor->place.set(50,50,0);

	level->incActor(mainactor);

	cams.push_back( new IScrollCamera(mainactor,new CRectangle(0,0,320,240),this) );

	printf("onInit : IScrollObjects %d \n",IScrollObject::getInstances());
	

	// TODO : Inicialización de recursos no cargados necesarios en 'onIdle'

	return EXITO;

}

//idle. Main loop.
int IFirstEngine::onIdle()	{

	void (IFirstEngine::* fptr)(SDL_Event* e);

	if (!mainactor)
		return FRACASO;

	setIfTrue(move[i_up],mainactor->place.Y(),LambdaInc(-6));
	setIfTrue(move[i_down],mainactor->place.Y(),LambdaInc(6));
	setIfTrue(move[i_left],mainactor->place.X(),LambdaInc(-6));
	setIfTrue(move[i_right],mainactor->place.X(),LambdaInc(6));

	/*if (move[i_up])
		mainactor->place.Y()-=6;

	if (move[i_down])
		mainactor->place.Y()+=6;

	if (move[i_left])
		mainactor->place.X()-=6;

	if (move[i_right])
		mainactor->place.X()+=6;*/

	setIfTrue(mainactor->place.X(),cams[0]->getArea()->X() + cams[0]->getArea()->W() / 2 +32,LambdaLess);
	setIfTrue(mainactor->place.Y(),cams[0]->getArea()->Y() + cams[0]->getArea()->H() / 2 +32,LambdaLess);

	return EXITO;

}

int IFirstEngine::onExit() {

	for (auto i = cams.begin(); i != cams.end(); ++i) {
		auto level = dynamic_cast<IScrollLevel*>((*i)->getUniverse());
		auto actor = dynamic_cast<IScrollObject*>((*i)->Target());
		delete *i;
		if (level) CMultiverse.erase(level);
	}

	mainactor = NULL;

	printf("onExit : IScrollObjects %d \n",IScrollObject::getInstances());

	cams.clear();

	// TODO : Liberación de recursos no necesarios mientras no haya ejecución de 'onIdle'.

	CEngine::onExit();

	return EXITO;

}

void IFirstEngine::deselect() {
	CEngine::deselect();

	// TODO : Siempre se debe llamar previamente al método de la clase base.

}

int IFirstEngine::loop() {

	// TODO : Siempre se debe llamar posteriormente al método de la clase base.

	return CEngine::loop();
}

	 
int IFirstEngine::onMessage(Uint32 MsgID,MSGPARM Parm1,MSGPARM Parm2) {

	// TODO : Gestión de mensajes

	return EXITO;

}

	 
void IFirstEngine::pendingMessage(Uint32 MsgID,MSGPARM Parm1,MSGPARM Parm2) {

	// TODO : Gestión de mensajes

}