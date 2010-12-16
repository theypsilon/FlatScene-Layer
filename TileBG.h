#ifndef __TILE_BG__
#define __TILE_BG__

#include "SDL/SDL.h"
#include <string>
using namespace std;

typedef struct{
	Uint8 graph;
	Uint8 fileGraph;
	Uint16 flags;
	Uint8 dur;
	Uint8 fileDur;
	Uint16 vacio;
}TileBG;

typedef struct{
	string destino;
	Uint32 regionx1;
	Uint32 regionx2;
	Uint32 regiony1;
	Uint32 regiony2;
	Uint32 regionz;
	Uint32 destino_scroll_x;                
	Uint32 destino_scroll_y;
	Uint32 destino_scroll_z;
}gate;

#endif
