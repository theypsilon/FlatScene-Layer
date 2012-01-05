#ifndef __LAYER_BACKGROUNDS__
#define __LAYER_BACKGROUNDS__

#include "FSMessageHandler.h"
#include "FSActor.h"
#include "FSSpriteSet.h"
#include "FSRectangle.h"
#include "FSImage.h"
#include "FSColor.h"
#include "FSControlImages.h"
#include "Map.h"
#include <math.h>
#include <vector>
#include <list>
#include "TileBG.h"

class CLayerBG : public FSMessageHandler {
public:
	FSRectangle* area;
	FSRectangle* clipping;
	Uint8 size; // 100 = 100% del tama�o original.
	CMap* mapa; // 
	Uint8 flags; // Espejado, girado...
	Uint8 alpha;  // Transparencias...
//public:
	CLayerBG(CMap* mapa,FSRectangle* area,FSMessageHandler * pmhParent=NULL);
	virtual ~CLayerBG();
	virtual void init(float zoom=1.0)=0;
	virtual void clear();
	virtual void refresh(int cx, int cy)=0;

	void setSize(Uint8 size);
	Uint8 getSize();
	void setFlags(Uint8 flags);
	Uint8 getFlags();
	void setAlpha(Uint8 alpha);
	Uint8 getAlpha();

#ifdef MENSAJES_MSGIDS
	int SendMessage(Uint32 MsgID,MSGPARM ParmMsg=NULL);
#endif
};

typedef vector<CLayerBG*> LayerBGCollection;

class CLayerDinamic : public CLayerBG { 
private:
	Uint8 num_tiles;
	TileBG* tile; // Array[num_tiles];
public:
	CLayerDinamic(CMap* mapa,FSRectangle* area,Uint8 num_tiles,FSMessageHandler * pmhParent=NULL);
	virtual ~CLayerDinamic();
	virtual FSColor* getPixel(Uint8 i,Uint8 w,Uint8 h); 
	virtual void init(float zoom=1.0);
	virtual void refresh();
};

class CLayerUniform : public CLayerBG {
private:
	TileBG** tile; // Array[tam_w][tam_h];
	Uint8 tile_w; // por defecto, 2^5=32
	Uint8 tile_h; // por defecto, 2^5=32
	Uint8 tam_w; // SCREEN_W/tile_w+2;
	Uint8 tam_h; // SCREEN_H/tile_h+2;
public:
	CLayerUniform(CMap* mapa,FSRectangle* area,TileBG** tile,FSMessageHandler * pmhParent=NULL);
	virtual ~CLayerUniform();
	Uint8 getTile_W();
	Uint8 getTile_H();
	Uint8 getTam_W();
	Uint8 getTam_H();
	virtual void init(float zoom=1.0);
	virtual void refresh(int cx, int cy);
	virtual FSColor* getPixel(Uint8 i,Uint8 j,Uint8 w,Uint8 h); 
};

class CLayerInf : public CLayerUniform { 
private:
	bool pisable; // Hay alg�n elemento de la capa accesible a los actores.
public:
	CLayerInf(CMap* mapa,FSRectangle* area,TileBG** tile,bool pisable,FSMessageHandler * pmhParent=NULL);
	CLayerInf(CMap* mapa,FSRectangle* area,TileBG** tile,FSMessageHandler * pmhParent=NULL);
	virtual ~CLayerInf();
	bool isPisable();
};

class CLayerSup : public CLayerUniform { 
private:
	Uint8 desfase; // Cuanto m�s cerca, m�s r�pido se mueven las tiles.	
public:
	CLayerSup(CMap* mapa,FSRectangle* area,TileBG** tile,FSMessageHandler * pmhParent=NULL);
	virtual ~CLayerSup();
};

class CLayerFondo : public CLayerUniform { 
private:
	Uint8 desfase; // Cuanto m�s lejos, m�s lento se mueven las tiles.
public:
	CLayerFondo(CMap* mapa,FSRectangle* area,TileBG** tile,FSMessageHandler * pmhParent=NULL);
	virtual ~CLayerFondo();
};

class CFloor : public FSMessageHandler {
private:
	FSRectangle* area;
	bool capaSuperior(CLayerBG* layer);
	CMap* mapa;
	int id;
public:
	Uint32 numLayers;
	LayerBGCollection Layer; // N

	CFloor(FSRectangle* area, CMap* mapa,int id,FSMessageHandler * pmhParent=NULL);
	virtual ~CFloor();
	virtual void refresh(int cx, int cy);
	virtual inline LayerBGCollection& getLayers() { return Layer; }

#ifdef MENSAJES_MSGIDS
	int SendMessage(Uint32 MsgID,MSGPARM ParmMsg=NULL);
#endif
};

typedef vector<CFloor*> FloorCollection;
/**/
#endif
