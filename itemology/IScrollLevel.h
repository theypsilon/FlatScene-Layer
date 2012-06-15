#ifndef _I_SCROLL_LEVEL_
#define _I_SCROLL_LEVEL_

#include "FSLibrary.h"

#include "IScrollObject.h"
#include "Tile.h"

struct TileBG {
    unsigned char   graph;
    unsigned char   fileGraph;
    unsigned short  flags;
    unsigned char   dur;
    unsigned char   fileDur;
    unsigned short  vacio;
};

struct ITileAndDur {
    std::vector<
    std::vector<Tile> > tiles;
    bool                dur;
};

struct gate {
    std::string     destino;
    unsigned int    regionx1;
    unsigned int    regionx2;
    unsigned int    regiony1;
    unsigned int    regiony2;
    unsigned int    regionz;
    unsigned int    destino_scroll_x;
    unsigned int    destino_scroll_y;
    unsigned int    destino_scroll_z;
};



class IScrollLevel : public Universe {
    friend class IScrollCamera;

    unsigned int                    mapHeight;
    unsigned int                    mapWidth;
    unsigned int                    numLayers;

    // dimension de tiles.
    unsigned int                    tileHeight;
    unsigned int                    tileWidth;

    std::string                     datosTile;
    std::string                     datosSong;

    std::vector<Spriteset>          _tilesets;
    std::vector<Spriteset>          _collisionsets;

    std::vector<ITileAndDur>        _layerlvl;
    std::vector<
    std::vector<
    std::vector<Tile>> const*>      _linktodur;

    std::vector<Uint32>             LayerFloor;
    std::vector<Uint32>             LayerType;

    Uint32                          numGates;

    std::vector<gate>               Gates;

    std::vector<
    std::vector<
    std::vector<
    std::list  <IScrollObject*>>>>  _MA;

public:
                                    IScrollLevel(std::string name);
                                    ~IScrollLevel();

    virtual void                    load();
    virtual void                    unload();

    virtual int                     incActor(Actor* act);
    virtual int                     decActor(Actor* act);

    virtual unsigned int            getPixel(int x, int y, int z);

    virtual unsigned int            getW() { return mapWidth; };
    virtual unsigned int            getH() { return mapHeight; };
    virtual unsigned int            getTileW() { return tileWidth; };
    virtual unsigned int            getTileH() { return tileHeight; };

    const std::vector<Spriteset>&   getTileset() { return _tilesets; };
};

#endif
