// Library.cpp: implementation of the CLibrary class.
//
//////////////////////////////////////////////////////////////////////

#ifdef WIN32
#ifdef VISUAL_LEAKS
	#include "vld.h"
	#include "vldapi.h"
#endif
#endif

#include "Library.h"

#define EXITENGINE(A); if (A && dynamic_cast<CEngine*>(A)) if (A->isInitialized()) { CEngine* eaux = getActualEngine(); setActualEngine(A); A->onExit(); setActualEngine(eaux); }
#define INITENGINE(A); if (A && dynamic_cast<CEngine*>(A)) if (A->isInitialized()) { CEngine* eaux = getActualEngine(); setActualEngine(A); A->onInit(); setActualEngine(eaux); }
#define KILLENGINE(A); EXITENGINE(A); if (A) { delete A; A=NULL; }

Uint32 CLibrary::MSGID_Exit=CMessageHandler::getNextMSGID();
Uint32 CLibrary::MSGID_Restart=CMessageHandler::getNextMSGID();
Uint32 CLibrary::MSGID_RunEngine=CMessageHandler::getNextMSGID();
Uint32 CLibrary::MSGID_ReloadEngine=CMessageHandler::getNextMSGID();
Uint32 CLibrary::MSGID_ChangeEngine=CMessageHandler::getNextMSGID();
Uint32 CLibrary::MSGID_KillEngine=CMessageHandler::getNextMSGID();


CLibrary* CLibrary::s_pTheLibrary=NULL;

list<string> CLibrary::errors;


void CLibrary::setLibrary(CLibrary* pTheLib)
{
	
	if(s_pTheLibrary!=NULL)
	{
		
		Error("More than one CLibrary created!",TE_controlViolation);
		
		return;
	}
	
	s_pTheLibrary=pTheLib;

#ifdef DEBUGTEST
	CLibrary::debugticks=Chrono.getTick();
#endif
}

void CLibrary::setActualEngine(CEngine *newEngineActive) {
	getLibrary()->actualEngine = newEngineActive;
}


CLibrary::CLibrary(bool xmlconfig):
CMessageHandler(NULL)
{
#ifdef IN_FILE_ERROR
	CLibrary::errorsInSession = false;
#endif
	
	setLibrary(this);

	setActualEngine(NULL);
	
	if(SDL_Init(SDL_INIT_TIMER)==-1)
	{
		
		Error("Failed to Init SDL:",TE_SDL_MSG);
			
		return;
	}
	
	atexit(SDL_Quit);
	atexit(onExit);

	if (xmlconfig) {

		bool fullscreen= false;

		int res_x = RESOLUCION_X;
		int res_y = RESOLUCION_Y;
		int bpp = BITMODE;


		TiXmlDocument xmldoc(resource("config.xml").c_str());
		if (xmldoc.LoadFile())	{

			TiXmlHandle input(xmldoc.FirstChildElement("System")); TiXmlElement* node;

			if (node = input.FirstChildElement("fullscreen").ToElement()) {
				if (node->Attribute("option") && strcmp(node->Attribute("option"),"yes")==0)
					fullscreen = true;
			}

			if (node = input.FirstChildElement("mode").ToElement()) {
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

		CScreen::start(res_x,res_y,bpp,fullscreen,doublebuff);
	}

}


CLibrary::CLibrary ( int width , int height , int bpp , bool fullscreen, bool doublebuff ) :
CMessageHandler(NULL)
{
#ifdef IN_FILE_ERROR
	CLibrary::errorsInSession = false;
#endif
	
	setLibrary(this);

	setActualEngine(NULL);

	
	if(SDL_Init(SDL_INIT_TIMER)==-1)
	{
		
		Error("Failed to Init SDL:",TE_SDL_MSG);
		
		return;
	}
	
	atexit(SDL_Quit);
	atexit(onExit);

	srand(rand());

	CScreen::start(width,height,bpp,fullscreen,doublebuff);

}

int CLibrary::startLibrary(bool xmlconfig)
{
	if (s_pTheLibrary!=NULL)	{
		Error("La biblioteca ya ha sido inicializada.");
		return FRACASO;
	}

	int num_e = errors.size();

	new CLibrary(xmlconfig);

	if (errors.size()!= num_e)
		return FRACASO;

	CScreen::initProcRenders();

	return EXITO;
}

int CLibrary::startLibrary( int width , int height , int bpp , bool fullscreen, bool doublebuff )
{
	if (s_pTheLibrary!=NULL)	{
		Error("La biblioteca ya ha sido inicializada.");
		return FRACASO;
	}

	int num_e = errors.size();

	new CLibrary(width,height,bpp,fullscreen,doublebuff);

	if (errors.size()!= num_e)
		return FRACASO;

	CScreen::initProcRenders();

	return EXITO;
}


CLibrary::~CLibrary()
{
}


void CLibrary::onExit()
{

	while( !getLibrary()->engineIn.empty() ) {

		CEngine * engine = *getLibrary()->engineIn.begin();

		getLibrary()->engineOut.remove(engine);

		setActualEngine(engine);
		if (engine->isInitialized())
			engine->onExit();

		getLibrary()->engineIn.erase(getLibrary()->engineIn.begin());

		delete engine;
	}

	while( !getLibrary()->engineOut.empty() ) {

		CEngine * engine = *getLibrary()->engineOut.begin();

		setActualEngine(engine);
		if (engine->isInitialized())
			engine->onExit();

		getLibrary()->engineOut.erase(getLibrary()->engineOut.begin());

		delete engine;
	}

	setActualEngine(NULL);

	Write.clear();

	
	CScreen::quit();

#ifdef IN_FILE_ERROR
	if (errorsInSession) {
		FILE* f=fopen("error.log","a+");
		if (f) {
			fprintf(f,"Session with errors finished without runtime collapse.\n");
			fclose(f);
		}
	}
#endif

}


int CLibrary::processEngines() {
	
	if(!s_pTheLibrary)
	{		
		Error("Biblioteca no inicializada");
		
		return FRACASO;
	}

	setActualEngine(NULL);

	// Selección del CEngine a ejecutar entre el Conjunto de CEngine.

	getLibrary()->engineIn.sort(CLibrary::orderEngine);

	for (list<CEngine*>::iterator it = getLibrary()->engineIn.begin(), jt = getLibrary()->engineIn.end();
		it != jt && getActualEngine()==NULL; 
		++it) {

		if (!(*it)->done) {
			(*it)->done = true;
			setActualEngine(*it);
		}

	}


	if (getActualEngine() == NULL) {

		for (list<CEngine*>::iterator it = getLibrary()->engineIn.begin(), jt = getLibrary()->engineIn.end();	it != jt; 	++it) 
			(*it)->done = false;

		setActualEngine(getLibrary()->engineIn.front());
		getLibrary()->engineIn.front()->done=true;
	}

	// Ejecución del CEngine seleccionado	
	
	while (getActualEngine()) {

		if (!getActualEngine()->isInitialized())
			if (!getActualEngine()->onInit()) {
				Error("Could not initialize new interface!");
				return(-1);
			}

		getActualEngine()->loop();

		getLibrary()->readMessages();
	}
	
	return EXITO;
}

CEngine* CLibrary::getActualEngine() {

	return (getLibrary()?getLibrary()->actualEngine:NULL);

}

int CLibrary::addEngine(CEngine* engine,int priority) {

	if (s_pTheLibrary==NULL || engine == NULL)
		return FRACASO;

	engine->priority = priority;
	getLibrary()->engineIn.push_back(engine);

	return EXITO;
}

int CLibrary::onMessage(Uint32 MsgID,MSGPARM Parm1,MSGPARM Parm2) {

	if ( MsgID >= MSGID_Exit && MsgID <= MSGID_KillEngine ) {

		if (actualEngine)
			actualEngine->deselect();

		bufferMessages.push_back(new CMessage(MsgID,Parm1,Parm2));

		return EXITO;

	}	else return(CMessageHandler::onMessage(MsgID,Parm1,Parm2));
}

void CLibrary::pendingMessage(Uint32 MsgID, MSGPARM Parm1, MSGPARM Parm2) {

	if (MsgID==MSGID_Exit) {

		setActualEngine(NULL);

	} else if (MsgID==MSGID_Restart) {

		for (list<CEngine*>::iterator it = getLibrary()->engineIn.begin(), jt = getLibrary()->engineIn.end();	it != jt; 	++it) {
			CEngine* engine = *it;
			setActualEngine(engine);
			engine->done = false;
			if (engine->isInitialized())
				engine->onExit();
		}

		getLibrary()->engineIn.sort(CLibrary::orderEngine);

		setActualEngine(engineIn.front());
		engineIn.front()->done = true;

		Write.clear();
		Write.loadFont("tahoma");

#ifdef WIN32
		system("CLS");
#else
		system("clear");
#endif

#ifdef LOG_SISTEMA
	printf("Se ha reiniciado la aplicacion.\n\n");
#endif

	} else if (MsgID==MSGID_RunEngine) {

		CEngine* engine = (CEngine*)Parm1;

		if (dynamic_cast<CEngine*>(engine)) {

			bool find = false;

			for (list<CEngine*>::iterator it = getLibrary()->engineIn.begin(), jt = getLibrary()->engineIn.end();it!=jt;++it)
				find = find || engine == *it;

			for (list<CEngine*>::iterator it = getLibrary()->engineOut.begin(), jt = getLibrary()->engineOut.end();it!=jt;++it)
				find = find || engine == *it;

			if (!engine) {
				engineOut.push_back(engine);
			}

			setActualEngine(engine); 

		}

	} else if (MsgID==MSGID_ReloadEngine) {

		CEngine* engine = (CEngine*) Parm1;

		if (dynamic_cast<CEngine*>(engine)) {

			setActualEngine(engine);

			Write.erase();

			if (actualEngine && actualEngine->isInitialized())
				actualEngine->onExit();
		}

	} else if (MsgID==MSGID_ChangeEngine) {


		setActualEngine(NULL);

		getLibrary()->engineIn.sort(CLibrary::orderEngine);

		for (list<CEngine*>::iterator it = getLibrary()->engineIn.begin(), jt = getLibrary()->engineIn.end();
			it != jt && getActualEngine()==NULL; 
			++it) {

			if (!(*it)->done) {
				(*it)->done = true;
				setActualEngine(*it);
			}

		}

		if (getActualEngine() == NULL) {
			for (list<CEngine*>::iterator it = getLibrary()->engineIn.begin(), jt = getLibrary()->engineIn.end();	it != jt; 	++it) 
				(*it)->done = false;

			setActualEngine(getLibrary()->engineIn.front());
			getLibrary()->engineIn.front()->done=true;
		}

	} else if (MsgID==MSGID_KillEngine) {

		CEngine* engine = (CEngine*)Parm1;

		if (dynamic_cast<CEngine*>(engine)) {

			CEngine* act = getActualEngine(); // Lo salvamos para recuperarlo al final. 

			setActualEngine(engine); // Ponemos este engine de actual, por si pudiera haber conflictos a la hora de matarlo con el gestor del Multiverso y Imágenes.

			for (list<CEngine*>::iterator it = engineIn.begin(), jt = engineIn.end(); it!=jt; ++it) 
				if (engine == *it) {
					engineIn.erase(it);
					break;
				}

			if (engine->isInitialized())
				engine->onExit();

			delete engine;

			setActualEngine(act); // Recuperamos el engine actual.

			if (actualEngine == engine)
				setActualEngine(NULL);

			engine = NULL;

		}

	}
}

void CLibrary::Error (const char* c,TypeError e) {
	Error(std::string(c),e);
}

void CLibrary::Error (std::string s,TypeError e) {

	if (s == "") 
		s = "empty";

	if (e==TE_fileExists) {
		s="|-| ERROR : No existe tal fichero. +: "+s;
	} else if (e==TE_standard) {
		s="|-| ERROR : Standard -> "+s;
	} else if (e==TE_controlViolation) {
		s="|-| ERROR : Violancíon del control de la librería. +: "+s;
	} else if (e==TE_SDL_NOMSG) {
		s="|-| SDLError unknown. +: "+s;
	} else if (e==TE_SDL_MSG) {
		s= "|-| SDLError -> "+std::string(SDL_GetError())+" +: "+s;
		SDL_ClearError();
	} else if (e==TE_OPENGL_NOMSG) {
		s="|-| OpenGLError unknown. +: "+s;
	} else if (e==TE_OPENGL_MSG) {
		s= "|-| OpenGLError -> "+toStringErrorGL(glGetError())+" +: "+s;
	}

	errors.push_back(s);

#ifdef INSTANT_PRINT_ERROR
	s ="\n"+ s + "\n";
	fprintf(stderr,s.c_str());
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
			if (!getLibrary()->errorsInSession) {
				s="Session with errors start"+s;
				getLibrary()->errorsInSession=true;
			}
			fprintf(f,s.c_str());
			fclose(f);
		}

		getLibrary()->SendMessage(MSGID_Exit);

	} 
#endif
}
void CLibrary::Error (char* c,TypeError e) {
	Error(std::string(c),e);
}

string CLibrary::toStringErrorGL(GLenum e) {
	string s ="";

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

string CLibrary::readLastError() {
	if (errors.empty())
		return SINERROR;
	else
		return errors.back();
}

string CLibrary::popError() {
	if (errors.empty())
		return SINERROR;
	else {
		string ret = errors.back();
		errors.pop_back();
		return ret;
	}
}

//get singleton
CLibrary* CLibrary::getLibrary()
{
	//return static pointer to application
	return(s_pTheLibrary);
}

#ifdef DEBUGTEST

bool CLibrary::debugging=false;

int CLibrary::debugticks=0;

void CLibrary::debug(bool startdebug,const char* warning) {
	if (startdebug) {
		if (!CLibrary::debugging) {
			debugticks=Chrono.getTick();
			fprintf(stderr,"\n** (+) La aplicacion ha entrado en estado de debug **\n");
		}
		if (warning)
			fprintf(stderr,"DEBUG: %s\n",warning);
		CLibrary::debugging = true;	// Siempre debe haber aquí un Breakpoint para el Visual Studio.
	}
}

bool CLibrary::inDebug() {
	if (CLibrary::debugging)
		if (Chrono.getTick() > debugticks + DEBUGTESTTICKS) {
			fprintf(stderr,"\n** (-) La aplicacion ha salido del estado de debug **\n");
			CLibrary::debugging=false;
		}
	return CLibrary::debugging;
}

#endif

bool CLibrary::orderEngine(CEngine* x, CEngine* y) {

	return ((x->priority)<(y->priority));

}



string resource(const char* c) {
	string s = c;
	s = "resources/"+s;
	return (s);
}
