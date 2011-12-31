#ifndef __TIME_H__
#define __TIME_H__

#include <map>
#include "SDL.h"
#include "FSdefinitions.h"

class FSEngine;

using namespace std;

class FSTime {
private:
	struct STimeData {
		int msInterval;
		int msLast;
		int frameCount;

		STimeData() : msInterval(16), msLast(0), frameCount(0) {
		};
	};

	map<FSEngine*,STimeData> fc;

#ifdef MENSAJES_FPS
	int fps;
	int auxTimer;
	map<FSEngine*,int> adminText;
#endif

	STimeData* actTime;
	FSEngine* admin;

	bool all;
	int allMsInterval;
public:
	FSTime();
	~FSTime();
	int setInterval(int msNew, bool all=false);
	int setFPS(int fpsNew, bool all=false);
	int nextFrame();
	int getTick();

	bool isTimeForAll();
};

extern FSTime Chrono;

#endif
