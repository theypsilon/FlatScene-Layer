#ifndef _I_SCROLL_LEVEL_
#define _I_SCROLL_LEVEL_

#include "FSLibrary.h"

#include "IScrollObject.h"

typedef struct{
	Uint8 graph;
	Uint8 fileGraph;
	Uint16 flags;
	Uint8 dur;
	Uint8 fileDur;
	Uint16 vacio;
}TileBG;

typedef struct {
	TileBG** tile;
	bool dur;
}ITileAndDur;

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



class IScrollLevel :
	public CUniverse
{
private:

	Uint32 mapHeight;
	Uint32 mapWidth;
	Uint32 numLayers;

	// dimension de tiles.
	Uint32 tileHeight;
	Uint32 tileWidth;

	// Primer tileset del mapa :
	Uint32 tileSet;
	Uint32 durezaSet;
	// Úlitmo tileset del mapa :
	Uint32 lastTileset;
	Uint32 lastDurezaset;

	string datosTile;
	string datosSong;

	vector<ITileAndDur> layerlvl;
	vector<TileBG**> linktodur;

	vector<Uint32> LayerFloor;
	vector<Uint32> LayerType;

	Uint32 numGates;

	vector<gate> Gates;

	vector<IScrollObjectCollection***> MA;

	friend class IScrollCamera;

public:
	IScrollLevel(string name);
	~IScrollLevel(void);
	
	virtual void load();
	virtual void unload();

	virtual int incActor(CActor* act);
	virtual int decActor(CActor* act);
	
	virtual Uint32 getPixel(int x, int y, int z);

	virtual Uint32 getW() { return mapWidth; };
	virtual Uint32 getH() { return mapHeight; };
	virtual Uint32 getTileW() { return tileWidth; };
	virtual Uint32 getTileH() { return tileHeight; };
	virtual Uint32 getTileset() { return tileSet; };
};

#endif