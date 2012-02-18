#include "IFirstEngine.h"

IFirstEngine::IFirstEngine() {

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

	for (auto i = cams.begin(); i != cams.end(); ++i) {
		(*i)->render();
	}

	Write.render();

	return EXITO;
}

int  IFirstEngine::onInit() {

	FSEngine::onInit();


	IScrollLevel* level = new IScrollLevel("resources/mapa1");

	CMultiverse.add(level);

	level->load();

	mainactor = new IScrollObject();

	mainactor->place.set(50,50,0);

	level->incActor(mainactor);

	cams.push_back( new IScrollCamera(mainactor,new FSRectangle(0,0,320,240)) );

	printf("onInit : IScrollObjects %d \n",IScrollObject::getInstances());

	return EXITO;

}

//idle. Main loop.
int IFirstEngine::onIdle()	{

	void (IFirstEngine::* fptr)(SDL_Event* e);

	if (!mainactor)
		return FRACASO;

    if (move[i_up])    mainactor->place.y += -6;
    if (move[i_down])  mainactor->place.y += +6;
    if (move[i_left])  mainactor->place.x += -6;
    if (move[i_right]) mainactor->place.x += +6;


	//setIfTrue(mainactor->place.x,cams[0]->getArea()->x + cams[0]->getArea()->w / 2 +32,LambdaLess);
	//setIfTrue(mainactor->place.y,cams[0]->getArea()->y + cams[0]->getArea()->h / 2 +32,LambdaLess);

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

	FSEngine::onExit();

	return EXITO;

}

void IFirstEngine::deselect() {
	FSEngine::deselect();

	// TODO : Siempre se debe llamar previamente al método de la clase base.

}

int IFirstEngine::loop() {

	// TODO : Siempre se debe llamar posteriormente al método de la clase base.

	return FSEngine::loop();
}
