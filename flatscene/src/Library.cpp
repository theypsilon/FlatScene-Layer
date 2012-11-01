#include "Library.h"
#include "Includes.h"
#include "Screen.h"
#include "parserXML.h"

#include "Exception.h"
#include <algorithm>

#define RESOLUCION_X 320
#define RESOLUCION_Y 240
#define BITMODE 32

namespace FlatScene {


// int startLibrary(bool xmlconfig) {

//     if (xmlconfig) {

//         bool fullscreen= false;

//         int res_x = RESOLUCION_X;
//         int res_y = RESOLUCION_Y;
//         int bpp = BITMODE;


//         TiXmlDocument xmldoc("config.xml");
//         if (!xmldoc.LoadFile()) throw FileDoesntExistException("config.xml");

//         TiXmlHandle input(xmldoc.FirstChildElement("System")); TiXmlElement* node;
//         node = input.FirstChildElement("fullscreen").ToElement();
//         if (node) {
//             if (node->Attribute("option") && strcmp(node->Attribute("option"),"yes")==0)
//                 fullscreen = true;
//         }
//         node = input.FirstChildElement("mode").ToElement();
//         if (node) {
//             if (node->Attribute("width"))
//                 node->QueryIntAttribute("width",&res_x);
//             if (node->Attribute("height"))
//                 node->QueryIntAttribute("height",&res_y);
//             if (node->Attribute("bpp"))
//                 node->QueryIntAttribute("bpp",&bpp);
//         }

//         bool doublebuff=true;

//         srand(rand());

//         return Screen::I().start(res_x,res_y,bpp,fullscreen,doublebuff);

//     }

//     return EXIT_SUCCESS;
// }

/*
std::vector<std::unique_ptr<Engine>> Library::processEngine(std::unique_ptr<Engine>&& eng) {

    std::vector<std::unique_ptr<Engine>> veng;
    veng.push_back(std::move(eng));
    processEngine(veng);
    return veng;
}

void Library::processEngine(std::vector<std::unique_ptr<Engine>>& veng) {
    _impl->setActualEngine(nullptr);

    typedef std::unique_ptr<Engine> pEngine;

    _impl->sort(veng);

    for (auto it = veng.begin(), et = veng.end(); 
        it != et && getActualEngine() == nullptr; ++it) {

        Engine& eng = **it;
        if (!eng.done) {
            eng.done = true;
            _impl->setActualEngine(it->get());
        }
    }

    if (getActualEngine() == nullptr) {

        for (auto it = veng.begin(), et = veng.end(); it != et; ++it)
            (*it)->done = false;

        if (veng.empty()) {
            throw Exception("There are no engines in queue!");
        }

        _impl->setActualEngine(veng.front().get());
        veng.front()->done=true;
    }

    // Ejecuci�n del CEngine seleccionado

    while (auto actualEngine = (*_impl).actualEngine) {

        if (!actualEngine->isInitialized()) {
            actualEngine->onInit();
            if (!actualEngine->isInitialized())
                throw Exception("Could not initialize new interface!");
        }

        actualEngine->loop();

        for (auto it = _impl->endTasks.begin(); it != _impl->endTasks.end(); ++it) {
            (*it)();
        }
    }
}

std::vector<std::unique_ptr<Engine>> Library::processEngines() {
    processEngine((*_impl).engineIn);
    std::vector<std::unique_ptr<Engine>> veng;
    std::swap((*_impl).engineIn,veng);
    return veng;
}

int Library::addEngine(std::unique_ptr<Engine> engine,int priority) {

    if (engine == nullptr)
        return EXIT_FAILURE;

    engine->priority = priority;
    (*_impl).engineIn.push_back(std::move(engine));

    return EXIT_SUCCESS;
}

void Library::exit() {
    if ((*_impl).actualEngine)
        (*_impl).actualEngine->deselect();

    (*_impl).endTasks.push_back([&](){
        _impl->setActualEngine(nullptr);
    });
}

void Library::restart() {
    if ((*_impl).actualEngine)
        (*_impl).actualEngine->deselect();

    (*_impl).endTasks.push_back([&](){
        for (auto it = (*_impl).engineIn.begin(), jt = (*_impl).engineIn.end();    it != jt;   ++it) {
            std::unique_ptr<Engine>& engine = *it;
            _impl->setActualEngine(engine.get());
            engine->done = false;
            if (engine->isInitialized())
                engine->onExit();
        }

        _impl->sort((*_impl).engineIn);

        _impl->setActualEngine((*_impl).engineIn.front().get());
        (*_impl).engineIn.front()->done = true;

#ifdef WIN32
        system("CLS");
#else
        system("clear");
#endif

#ifdef LOG_SISTEMA
        printf("Se ha reiniciado la aplicacion.\n\n");
#endif
    });
}

void Library::runEngine(Engine* engine) {
    if ((*_impl).actualEngine)
        (*_impl).actualEngine->deselect();

    (*_impl).endTasks.push_back([&](){
        if (engine != nullptr)
            _impl->setActualEngine(engine);
    });
}

void Library::reloadEngine(Engine* engine) {
    if ((*_impl).actualEngine)
        (*_impl).actualEngine->deselect();

    (*_impl).endTasks.push_back([&](){
        if (engine != 0) {

            Engine* find = nullptr;

            for (auto it = (*_impl).engineIn.begin(), jt = (*_impl).engineIn.end();it!=jt;++it)
                if (engine == it->get())
                    find = engine;

            if (find == nullptr)
                if (engine == _impl->actualEngine)
                    find = engine;

            _impl->setActualEngine(find);

            if ((*_impl).actualEngine && (*_impl).actualEngine->isInitialized())
                (*_impl).actualEngine->onExit();
        }
    });
}

void Library::changeEngine() {
    if ((*_impl).actualEngine)
        (*_impl).actualEngine->deselect();

    (*_impl).endTasks.push_back([&](){
        _impl->setActualEngine(nullptr);

        _impl->sort((*_impl).engineIn);

        for (auto it = (*_impl).engineIn.begin(), jt = (*_impl).engineIn.end();
            it != jt && getActualEngine()==nullptr;
            ++it) {

            if (!(*it)->done) {
                (*it)->done = true;
                _impl->setActualEngine(it->get());
            }

        }

        if (getActualEngine() == nullptr) {
            for (auto it = (*_impl).engineIn.begin(), jt = (*_impl).engineIn.end();    it != jt;   ++it)
                (*it)->done = false;

            _impl->setActualEngine((*_impl).engineIn.front().get());
            (*_impl).engineIn.front()->done=true;
        }
    });
}

void Library::killEngine(Engine* engine) {
    if ((*_impl).actualEngine)
        (*_impl).actualEngine->deselect();

    (*_impl).endTasks.push_back([&](){
        if (engine != 0) {

            Engine* act = (*_impl).actualEngine; // Lo salvamos para recuperarlo al final.
            std::unique_ptr<Engine> find = nullptr;

            for (auto it = (*_impl).engineIn.begin(), jt = (*_impl).engineIn.end(); it!=jt; ++it) {
                if (engine == it->get()) {
                    find = std::move(*it);
                    (*_impl).engineIn.erase(it);
                    break;
                }
            }
        }
    });
}*/

} // flatscene
