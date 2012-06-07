#include "IFirstEngine.h"
#include "FSException.h"
#include <iostream>

IFirstEngine::IFirstEngine() {
    
    move[i_up] = false;
    move[i_down] = false;
    move[i_left] = false;
    move[i_right] = false;
    
    Write.loadFont("resources/tahoma");
    
}

IFirstEngine::~IFirstEngine()	{}

void IFirstEngine::onEvent(const SDL_Event& e) {

    Engine::onEvent(e);

    if ( SDLK_LEFT == e.key.keysym.sym ) {
        Library::I().exit();
    }
    
    bool turn = true;
    
    if (e.type == SDL_KEYUP)
        turn = false;
    
    if ( e.key.keysym.sym == SDLK_LEFT )
        move[i_left] = turn;
    
    if ( e.key.keysym.sym == SDLK_DOWN )
        move[i_down] = turn;
    
    if ( e.key.keysym.sym == SDLK_UP )
        move[i_up] = turn;
    
    if ( e.key.keysym.sym == SDLK_RIGHT )
        move[i_right] = turn;
    
    Write.lineOnce(0,10,10,"This is text!");
    
}

void IFirstEngine::drawFrame() {
    
    for (auto i = cams.begin(); i != cams.end(); ++i) {
        (*i)->render();
    }
    
    Write.render();
    
    Screen::I().render();
    
}

void  IFirstEngine::onInit() {
    
    Engine::onInit();
    
    
    IScrollLevel* level = new IScrollLevel("resources/mapa1");
    
    CMultiverse.add(level);
    
    level->load();
    
    mainactor = new IScrollObject();
    
    mainactor->place.set(50,50,0);
    
    level->incActor(mainactor);
    
    cams.push_back( new IScrollCamera(mainactor,new flatscene::Rectangle(0,0,320,240)) );
    
    printf("onInit : IScrollObjects %d \n",IScrollObject::getInstances());
    
}

//idle. Main loop.
void IFirstEngine::onIdle()	{
    
    //void (IFirstEngine::* fptr)(SDL_Event* e);
    
    if (!mainactor)
        throw Exception("no main actor",__LINE__);
    
    if (move[i_up])    mainactor->place.y += -6;
    if (move[i_down])  mainactor->place.y += +6;
    if (move[i_left])  mainactor->place.x += -6;
    if (move[i_right]) mainactor->place.x += +6;
    
    std::cout << "place.y" << mainactor->place.y << "place.x" << mainactor->place.x << "\r";
    
    
    //setIfTrue(mainactor->place.x,cams[0]->getArea()->x + cams[0]->getArea()->w / 2 +32,LambdaLess);
    //setIfTrue(mainactor->place.y,cams[0]->getArea()->y + cams[0]->getArea()->h / 2 +32,LambdaLess);
}

void IFirstEngine::onExit() {
    
    for (auto i = cams.begin(); i != cams.end(); ++i) {
        auto level = dynamic_cast<IScrollLevel*>((*i)->getUniverse());
        auto actor = dynamic_cast<IScrollObject*>((*i)->Target());
        delete *i;
        if (level) CMultiverse.erase(level);
    }
    
    mainactor = NULL;
    
    printf("onExit : IScrollObjects %d \n",IScrollObject::getInstances());
    
    cams.clear();
    
    Engine::onExit();
}

void IFirstEngine::deselect() {
    Engine::deselect();
}

void IFirstEngine::loop() {
    Engine::loop();
}
