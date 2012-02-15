// Library.cpp: implementation of the CLibrary class.
//
//////////////////////////////////////////////////////////////////////

#ifdef WIN32
#ifdef VISUAL_LEAKS
    #include "vld.h"
    #include "vldapi.h"
#endif
#endif

#include "FSLibraryImpl.h"
#include "FSScreenImpl.h"

#define EXITENGINE(A); if (A && dynamic_cast<CEngine*>(A)) if (A->isInitialized()) { CEngine* eaux = getActualEngine(); _impl->setActualEngine(A); A->onExit(); _impl->setActualEngine(eaux); }
#define INITENGINE(A); if (A && dynamic_cast<CEngine*>(A)) if (A->isInitialized()) { CEngine* eaux = getActualEngine(); _impl->setActualEngine(A); A->onInit(); _impl->setActualEngine(eaux); }
#define KILLENGINE(A); EXITENGINE(A); if (A) { delete A; A=NULL; }

Uint32 FSLibrary::MSGID_Exit=FSMessageHandler::getNextMSGID();
Uint32 FSLibrary::MSGID_Restart=FSMessageHandler::getNextMSGID();
Uint32 FSLibrary::MSGID_RunEngine=FSMessageHandler::getNextMSGID();
Uint32 FSLibrary::MSGID_ReloadEngine=FSMessageHandler::getNextMSGID();
Uint32 FSLibrary::MSGID_ChangeEngine=FSMessageHandler::getNextMSGID();
Uint32 FSLibrary::MSGID_KillEngine=FSMessageHandler::getNextMSGID();

void FSLibrary::LibraryImpl::setActualEngine(FSEngine *newEngineActive) {
    actualEngine = newEngineActive;
}

FSEngine* FSLibrary::getActualEngine() {
    return _impl->actualEngine;
}

FSLibrary::FSLibrary(): FSMessageHandler(NULL), _impl(new LibraryImpl) {
#ifdef IN_FILE_ERROR
    (*_impl).errorsInSession = false;
#endif
#ifdef DEBUGTEST
    (*_impl).debugging=false;
    (*_impl).debugticks=Chrono.getTick();
#endif

    (*_impl).actualEngine = NULL;

    if(SDL_Init(SDL_INIT_TIMER)==-1)
    {

        Error("Failed to Init SDL:",TE_SDL_MSG);

        return;
    }

    atexit(SDL_Quit);

}

int FSLibrary::startLibrary(bool xmlconfig) {

    if (xmlconfig) {

        bool fullscreen= false;

        int res_x = RESOLUCION_X;
        int res_y = RESOLUCION_Y;
        int bpp = BITMODE;


        TiXmlDocument xmldoc("config.xml");
        if (xmldoc.LoadFile())  {

            TiXmlHandle input(xmldoc.FirstChildElement("System")); TiXmlElement* node;
            node = input.FirstChildElement("fullscreen").ToElement();
            if (node) {
                if (node->Attribute("option") && strcmp(node->Attribute("option"),"yes")==0)
                    fullscreen = true;
            }
            node = input.FirstChildElement("mode").ToElement();
            if (node) {
                if (node->Attribute("width"))
                    node->QueryIntAttribute("width",&res_x);
                if (node->Attribute("height"))
                    node->QueryIntAttribute("height",&res_y);
                if (node->Attribute("bpp"))
                    node->QueryIntAttribute("bpp",&bpp);
            }

        } else {
            Error("config.xml",TE_fileExists);
        }

        bool doublebuff=true;

        srand(rand());

        return FSScreen::I().start(res_x,res_y,bpp,fullscreen,doublebuff);

    }

    return EXITO;
}

int FSLibrary::startLibrary( int width , int height , int bpp , bool fullscreen, bool doublebuff ) {
    return FSScreen::I().start(width,height,bpp,fullscreen,doublebuff);
}

void FSLibrary::LibraryImpl::onExit() {
    LibraryImpl* _impl = FSLibrary::I()._impl;
    while( !(*_impl).engineIn.empty() ) {

        FSEngine * engine = *(*_impl).engineIn.begin();

        (*_impl).engineOut.remove(engine);

        _impl->setActualEngine(engine);
        if (engine->isInitialized())
            engine->onExit();

        (*_impl).engineIn.erase((*_impl).engineIn.begin());

        delete engine;
    }

    while( !(*_impl).engineOut.empty() ) {

        FSEngine * engine = *(*_impl).engineOut.begin();

        _impl->setActualEngine(engine);
        if (engine->isInitialized())
            engine->onExit();

        (*_impl).engineOut.erase((*_impl).engineOut.begin());

        delete engine;
    }

    _impl->setActualEngine(NULL);

#ifdef IN_FILE_ERROR
    if ((*_impl).errorsInSession) {
        FILE* f=fopen("error.log","a+");
        if (f) {
            fprintf(f,"Session with errors finished without runtime collapse.\n");
            fclose(f);
        }
    }
#endif
}

FSLibrary::~FSLibrary() {
    delete _impl;
}

int FSLibrary::processEngines() {

    _impl->setActualEngine(NULL);

    // Selecci�n del CEngine a ejecutar entre el Conjunto de CEngine.

    (*_impl).engineIn.sort(FSLibrary::LibraryImpl::orderEngine);

    for (std::list<FSEngine*>::iterator it = (*_impl).engineIn.begin(), jt = (*_impl).engineIn.end();
        it != jt && getActualEngine()==NULL;
        ++it) {

        if (!(*it)->done) {
            (*it)->done = true;
            _impl->setActualEngine(*it);
        }

    }


    if (getActualEngine() == NULL) {

        for (std::list<FSEngine*>::iterator it = (*_impl).engineIn.begin(), jt = (*_impl).engineIn.end();    it != jt;   ++it)
            (*it)->done = false;

        if ((*_impl).engineIn.empty()) {
            FSLibrary::Error("There are no engines in queue!");
            return FRACASO;
        }

        _impl->setActualEngine((*_impl).engineIn.front());
        (*_impl).engineIn.front()->done=true;
    }

    // Ejecuci�n del CEngine seleccionado

    while (getActualEngine()) {

        if (!getActualEngine()->isInitialized())
            if (!getActualEngine()->onInit()) {
                Error("Could not initialize new interface!");
                return(-1);
            }

        getActualEngine()->loop();

        readMessages();
    }

    return EXITO;
}

int FSLibrary::addEngine(FSEngine* engine,int priority) {

    if (engine == NULL)
        return FRACASO;

    engine->priority = priority;
    (*_impl).engineIn.push_back(engine);

    return EXITO;
}

int FSLibrary::onMessage(Uint32 MsgID,MSGPARM Parm1,MSGPARM Parm2) {

    if ( MsgID >= MSGID_Exit && MsgID <= MSGID_KillEngine ) {

        if ((*_impl).actualEngine)
            (*_impl).actualEngine->deselect();

        bufferMessages.push_back(new FSMessage(MsgID,Parm1,Parm2));

        return EXITO;

    }   else return(FSMessageHandler::onMessage(MsgID,Parm1,Parm2));
}

void FSLibrary::pendingMessage(Uint32 MsgID, MSGPARM Parm1, MSGPARM Parm2) {

    if (MsgID==MSGID_Exit) {

        _impl->setActualEngine(NULL);

    } else if (MsgID==MSGID_Restart) {

        for (std::list<FSEngine*>::iterator it = (*_impl).engineIn.begin(), jt = (*_impl).engineIn.end();    it != jt;   ++it) {
            FSEngine* engine = *it;
            _impl->setActualEngine(engine);
            engine->done = false;
            if (engine->isInitialized())
                engine->onExit();
        }

        (*_impl).engineIn.sort(FSLibrary::LibraryImpl::orderEngine);

        _impl->setActualEngine((*_impl).engineIn.front());
        (*_impl).engineIn.front()->done = true;

        FSWriter::I().clear();
        FSWriter::I().loadFont("tahoma");

#ifdef WIN32
        system("CLS");
#else
        system("clear");
#endif

#ifdef LOG_SISTEMA
    printf("Se ha reiniciado la aplicacion.\n\n");
#endif

    } else if (MsgID==MSGID_RunEngine) {

        FSEngine* engine = (FSEngine*)Parm1;

        if (dynamic_cast<FSEngine*>(engine)) {

            bool find = false;

            for (std::list<FSEngine*>::iterator it = (*_impl).engineIn.begin(), jt = (*_impl).engineIn.end();it!=jt;++it)
                find = find || engine == *it;

            for (std::list<FSEngine*>::iterator it = (*_impl).engineOut.begin(), jt = (*_impl).engineOut.end();it!=jt;++it)
                find = find || engine == *it;

            if (!engine) {
                (*_impl).engineOut.push_back(engine);
            }

            _impl->setActualEngine(engine);

        }

    } else if (MsgID==MSGID_ReloadEngine) {

        FSEngine* engine = (FSEngine*) Parm1;

        if (dynamic_cast<FSEngine*>(engine)) {

            _impl->setActualEngine(engine);

            FSWriter::I().erase();

            if ((*_impl).actualEngine && (*_impl).actualEngine->isInitialized())
                (*_impl).actualEngine->onExit();
        }

    } else if (MsgID==MSGID_ChangeEngine) {


        _impl->setActualEngine(NULL);

        (*_impl).engineIn.sort(FSLibrary::LibraryImpl::orderEngine);

        for (std::list<FSEngine*>::iterator it = (*_impl).engineIn.begin(), jt = (*_impl).engineIn.end();
            it != jt && getActualEngine()==NULL;
            ++it) {

            if (!(*it)->done) {
                (*it)->done = true;
                _impl->setActualEngine(*it);
            }

        }

        if (getActualEngine() == NULL) {
            for (std::list<FSEngine*>::iterator it = (*_impl).engineIn.begin(), jt = (*_impl).engineIn.end();    it != jt;   ++it)
                (*it)->done = false;

            _impl->setActualEngine((*_impl).engineIn.front());
            (*_impl).engineIn.front()->done=true;
        }

    } else if (MsgID==MSGID_KillEngine) {

        FSEngine* engine = (FSEngine*)Parm1;

        if (dynamic_cast<FSEngine*>(engine)) {

            FSEngine* act = getActualEngine(); // Lo salvamos para recuperarlo al final.

            _impl->setActualEngine(engine); // Ponemos este engine de actual, por si pudiera haber conflictos a la hora de matarlo con el gestor del Multiverso y Im�genes.

            for (std::list<FSEngine*>::iterator it = (*_impl).engineIn.begin(), jt = (*_impl).engineIn.end(); it!=jt; ++it)
                if (engine == *it) {
                    (*_impl).engineIn.erase(it);
                    break;
                }

            if (engine->isInitialized())
                engine->onExit();

            delete engine;

            _impl->setActualEngine(act); // Recuperamos el engine actual.

            if ((*_impl).actualEngine == engine)
                _impl->setActualEngine(NULL);

            engine = NULL;

        }

    }
}

void FSLibrary::Error (const char* c,TypeError e) {
    Error(std::string(c),e);
}

void FSLibrary::Error (std::string s,TypeError e) {

    if (s == "")
        s = "empty";

    if (e==TE_fileExists) {
        s="|-| ERROR : No existe tal fichero. +: "+s;
    } else if (e==TE_standard) {
        s="|-| ERROR : Standard -> "+s;
    } else if (e==TE_controlViolation) {
        s="|-| ERROR : Violanc�on del control de la librer�a. +: "+s;
    } else if (e==TE_SDL_NOMSG) {
        s="|-| SDLError unknown. +: "+s;
    } else if (e==TE_SDL_MSG) {
        s= "|-| SDLError -> "+std::string(SDL_GetError())+" +: "+s;
        SDL_ClearError();
    } else if (e==TE_OPENGL_NOMSG) {
        s="|-| OpenGLError unknown. +: "+s;
    } else if (e==TE_OPENGL_MSG) {
        s= "|-| OpenGLError -> "+_impl->toStringErrorGL(glGetError())+" +: "+s;
    }

    (*_impl).errors.push_back(s);

#ifdef INSTANT_PRINT_ERROR
    s ="\n"+ s + "\n";
    fprintf(stderr,"%s",s.c_str());
#endif

#ifdef IN_FILE_ERROR
    if (getLibrary()) {

        if (s.at(0)!='\n')
            s ="\n"+ s + "\n";

        FILE* f=fopen("error.log","a+");
        if (!f) {
            s+="\tAnd errors.log couldn't open to register this.\n";
            fprintf(stderr,s.c_str());
        } else {
            fprintf(stderr,s.c_str());
            if (!(*_impl).errorsInSession) {
                s="Session with errors start"+s;
                (*_impl).errorsInSession=true;
            }
            fprintf(f,s.c_str());
            fclose(f);
        }

        SendMessage(MSGID_Exit);

    }
#endif
}
void FSLibrary::Error (char* c,TypeError e) {
    Error(std::string(c),e);
}

std::string FSLibrary::LibraryImpl::toStringErrorGL(GLenum e) {
    std::string s ="";

    switch (e) {
        case GL_NO_ERROR : s+="GL_NO_ERROR : No error has been recorded.";
        case GL_INVALID_ENUM : s+="GL_INVALID_ENUM : An unacceptable value is specified for an enumerated argument.";
        case GL_INVALID_VALUE : s+="GL_INVALID_VALUE : A numeric argument is out of range.";
        case GL_INVALID_OPERATION : s+= "GL_INVALID_OPERATION : The specified operation is not allowed in the current state.";
        case GL_STACK_OVERFLOW : s+= "GL_STACK_OVERFLOW : This command would cause a stack overflow.";
        case GL_STACK_UNDERFLOW : s+="GL_STACK_UNDERFLOW : This command would cause a stack underflow.";
        case GL_OUT_OF_MEMORY : s+="GL_OUT_OF_MEMORY : There is not enough memory left to execute the command. ";
    }

    return s;
}

std::string FSLibrary::readLastError() {
    if ((*_impl).errors.empty())
        return SINERROR;
    else
        return (*_impl).errors.back();
}

std::string FSLibrary::popError() {
    if ((*_impl).errors.empty())
        return SINERROR;
    else {
        std::string ret = (*_impl).errors.back();
        (*_impl).errors.pop_back();
        return ret;
    }
}

#ifdef DEBUGTEST

void FSLibrary::debug(bool startdebug,const char* warning) {
    if (startdebug) {
        if (!(*_impl).debugging) {
            debugticks=Chrono.getTick();
            fprintf(stderr,"\n** (+) La aplicacion ha entrado en estado de debug **\n");
        }
        if (warning)
            fprintf(stderr,"DEBUG: %s\n",warning);
        (*_impl).debugging = true;    // Siempre debe haber aqu� un Breakpoint para el Visual Studio.
    }
}

bool FSLibrary::inDebug() {
    if ((*_impl).debugging)
        if (Chrono.getTick() > debugticks + DEBUGTESTTICKS) {
            fprintf(stderr,"\n** (-) La aplicacion ha salido del estado de debug **\n");
            (*_impl).debugging=false;
        }
    return (*_impl).debugging;
}

#endif

bool FSLibrary::LibraryImpl::orderEngine(FSEngine* x, FSEngine* y) {

    return ((x->priority)<(y->priority));

}

#ifdef GLOBAL_SINGLETON_REFERENCES
FSLibrary& FSLib = FSLibrary::I();
#endif
