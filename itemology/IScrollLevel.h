#ifndef _I_SCROLL_LEVEL_
#define _I_SCROLL_LEVEL_

#include "IInclude.h"

#include "IScrollObject.h"
#include "Tile.h"

//typedef SmallTile Tile;

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

struct Gate {
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



class IScrollLevel : public Cinema::Universe {
    friend class IScrollCamera;

    unsigned int                    _mapHeight;
    unsigned int                    _mapWidth;
    unsigned int                    _numLayers;

    // dimension de tiles.
    unsigned int                    _tileHeight;
    unsigned int                    _tileWidth;

    std::string                     _datosTile;
    std::string                     _datosSong;

    std::vector<Spriteset>          _tilesets;
    std::vector<Spriteset>          _collisionsets;

    std::vector<ITileAndDur>        _layerlvl;
    std::vector<
    std::vector<
    std::vector<Tile>> const*>      _linktodur;

    std::vector<unsigned int>       _layerFloor;
    std::vector<unsigned int>       _layerType;

    unsigned int                    _numGates;

    std::vector<Gate>               _gates;

    std::vector<
    std::vector<
    std::vector<
    std::list  <IScrollObject*>>>>  _MA;

public:
                                    IScrollLevel(std::string name);
                                    ~IScrollLevel();

    virtual void                    load();
    virtual void                    unload();

    virtual int                     incActor(Cinema::Actor* act);
    virtual int                     decActor(Cinema::Actor* act);

    virtual unsigned int            getPixel(int x, int y, int z);

    virtual unsigned int            getW() { return _mapWidth; };
    virtual unsigned int            getH() { return _mapHeight; };
    virtual unsigned int            getL() { return _numLayers; };
    virtual unsigned int            getTileW() { return _tileWidth; };
    virtual unsigned int            getTileH() { return _tileHeight; };

    const std::vector<Spriteset>&   getTileset() { return _tilesets; };
};

#endif
