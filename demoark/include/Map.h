#ifndef __M_A_P__
#define __M_A_P__

#include <stdio.h>
#include "FSUniverse.h"
#include "TileBG.h"
#include "ActorScrollMap.h"
#include <list>
#include <string>

class CMap : public FSUniverse {
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
    // �litmo tileset del mapa :
    Uint32 lastTileset;
    Uint32 lastDurezaset;

    std::string datosTile;
    std::string datosSong;

    TileBG*** tile;
    TileBG*** dur;

    Uint32* LayerFloor;
    Uint32* LayerType;

    Uint32 numGates;

    Uint8 precissionPlus;
public:
    gate* Gates;

    ActorScrollCollection**** MA;

    CMap(std::string name);
    virtual ~CMap();

    virtual void load();
    virtual void unload();
    virtual std::string& getName();

    virtual int incActor(FSActor* act);
    virtual int decActor(FSActor* act);

    virtual Uint32 getW();
    virtual Uint32 getH();
    virtual Uint32 getTileW();
    virtual Uint32 getTileH();
    virtual TileBG*** getTiles();
    virtual TileBG*** getDurezas();
    virtual Uint32 getLayer();
    virtual Uint32* getLayerFloor();
    virtual Uint32* getLayerType();
    virtual Uint32 getGates() { return numGates; };
    virtual gate getGate(int n) { return Gates[n]; };
    virtual Uint32 getTileset();
    virtual Uint32 getDurezaset();
    virtual inline Uint32 getLastTileset();
    virtual inline Uint32 getLastDurezaset();

    virtual inline Uint8 getPrecissionPlus() const { return precissionPlus; }
    virtual inline Uint8 setPrecissionPlus(Uint8 n);

    virtual Uint32 getPixel(int x, int y, int z);

#ifdef MENSAJES_MSGIDS
    int SendMessage(Uint32 MsgID,MSGPARM ParmMsg=NULL);
#endif

    static void rescataChar(std::string& cad,int size,int n, FILE* f);
};


#endif
