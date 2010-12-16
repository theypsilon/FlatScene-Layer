#ifndef __TIME_H__
#define __TIME_H__

#include <map>
#include "SDL/SDL.h"
#include "definitions.h"

class CEngine;

using namespace std;

struct STimeData {
	int msInterval;
	int msLast;
	int frameCount;

	STimeData() : msInterval(16), msLast(0), frameCount(0) {
	};
};


class CTime {
private:
	map<CEngine*,STimeData> fc;

#ifdef MENSAJES_FPS
	int fps;
	int auxTimer;
	map<CEngine*,int> adminText;
#endif

	STimeData* actTime;
	CEngine* admin;

	bool all;
	int allMsInterval;
public:
	CTime();
	~CTime();
	int setInterval(int msNew, bool all=false);
	int setFPS(int fpsNew, bool all=false);
	int nextFrame();
	int getTick();

	bool isTimeForAll();
};

extern CTime Chrono;

#endif
