#ifndef __EVENTHANDLER_H__
#define __EVENTHANDLER_H__

#include "FSMessageHandler.h"
#include "FSTime.h"
#include <map>


class CLibrary;

class CEngine : public CMessageHandler
{
private:

	bool initialized;

	map<Uint8,const void*> eventHandlerRegister;

	friend class CLibrary;

protected:

	int priority;

	virtual int onInit();
	
	virtual int onExit();

	virtual int loop();

	
	virtual int onIdle();

	inline virtual void deselect();



public:
	bool done;

	
	CEngine(CMessageHandler* pmhParent=NULL);
	
	virtual ~CEngine();


	bool isInitialized();

	virtual int drawFrame();

	const void* setEventHandler(Uint8 type,void (eventHandler)(SDL_Event*));

};

#endif //#ifndef __EVENTHANDLER_H__
