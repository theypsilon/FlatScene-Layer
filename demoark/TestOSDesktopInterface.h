#ifndef  __TESTOSDESKTOPINTERFACE_H__
#define __TESTOSDESKTOPINTERFACE_H__

#include "FSLibrary.h"

#include "ActorMouse.h"
#include "CameraOS.h"

class CTestOSDesktopInterface : public CEngine  
{
private:

	CActor* raton, *fondo;
	CCamera* camaraf, *camarar;

	float xpos,ypos,zpos, lastx, lasty;

	float xrot,yrot;

	int file;

	int text;

	bool up,down,left,right;

public:

	CTestOSDesktopInterface(CMessageHandler * pmhParent=NULL);
	~CTestOSDesktopInterface();

	int onInit();
	int onExit();

	int drawFrame();

	int onIdle();

	int loop();

	static void onKeyTestOS(SDL_Event*);
	static void onMouseTestOS(SDL_Event*);
};

#endif