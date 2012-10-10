#include "IFirstEngine.h"
#include "Exception.h"
#include "Event.h"
#include <iostream>

IFirstEngine::IFirstEngine() {
    
    move[i_up] = false;
    move[i_down] = false;
    move[i_left] = false;
    move[i_right] = false;
    
}

IFirstEngine::~IFirstEngine()	{}

void IFirstEngine::onEvent(const Event& e) {

    Engine::onEvent(e);

    if ( Key::ESCAPE == e.getSymbol() ) {
        Library::I().exit();
    }
    
    bool turn = true;
    
    if (e.getType() == EventType::KEYUP)
        turn = false;
    
    if ( e.getSymbol() == Key::LEFT )
        move[i_left] = turn;
    
    if ( e.getSymbol() == Key::DOWN )
        move[i_down] = turn;
    
    if ( e.getSymbol() == Key::UP )
        move[i_up] = turn;
    
    if ( e.getSymbol() == Key::RIGHT )
        move[i_right] = turn;

    if ( e.getSymbol() == Key::F2 && turn) {
        FSDraw.ToggleFullscreen();
    }
    
}

void IFirstEngine::drawFrame() {
    
    for (auto camera : cams) {
        camera->render();
    }
    
    Screen::I().render();
    
}

void  IFirstEngine::onInit() {
    
    Engine::onInit();
    
    
    level.reset( new IScrollLevel("resources/mapa1") );
    
    level->load();
    
    mainactor = new IScrollObject();
    
    mainactor->place.set(50,50,0);
    
    level->incActor(mainactor);
    
    cams.push_back( new IScrollCamera(mainactor,new FlatScene::Rectangle(0,0,320,240)) );
    
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
        auto actor = dynamic_cast<IScrollObject*>((*i)->Target());
        delete *i;
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
