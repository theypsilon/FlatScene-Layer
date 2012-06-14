#ifndef _I_SCROLL_LEVEL_
#define _I_SCROLL_LEVEL_

#include "FSLibrary.h"

#include "IScrollObject.h"
#include "Tile.h"

struct TileBG {
    Uint8 graph;
    Uint8 fileGraph;
    Uint16 flags;
    Uint8 dur;
    Uint8 fileDur;
    Uint16 vacio;
};

struct ITileAndDur {
    std::vector<std::vector<Tile> > tiles;
    bool dur;
};

struct gate {
    std::string destino;
    Uint32 regionx1;
    Uint32 regionx2;
    Uint32 regiony1;
    Uint32 regiony2;
    Uint32 regionz;
    Uint32 destino_scroll_x;                
    Uint32 destino_scroll_y;
    Uint32 destino_scroll_z;
};



class IScrollLevel :
    public Universe
{
private:

    Uint32 mapHeight;
    Uint32 mapWidth;
    Uint32 numLayers;

    // dimension de tiles.
    Uint32 tileHeight;
    Uint32 tileWidth;

    std::string datosTile;
    std::string datosSong;

    std::vector<Spriteset> _tilesets;
    std::vector<Spriteset> _collisionsets;

    std::vector<ITileAndDur> _layerlvl;
    std::vector<
        std::vector<
            std::vector<Tile>> const*> _linktodur;

    std::vector<Uint32> LayerFloor;
    std::vector<Uint32> LayerType;

    Uint32 numGates;

    std::vector<gate> Gates;

    std::vector<
        std::vector<
            std::vector<
                std::list<IScrollObject*>>>> _MA;

    friend class IScrollCamera;

public:
    IScrollLevel(std::string name);
    ~IScrollLevel(void);

    virtual void load();
    virtual void unload();

    virtual int incActor(Actor* act);
    virtual int decActor(Actor* act);

    virtual Uint32 getPixel(int x, int y, int z);

    virtual Uint32 getW() { return mapWidth; };
    virtual Uint32 getH() { return mapHeight; };
    virtual Uint32 getTileW() { return tileWidth; };
    virtual Uint32 getTileH() { return tileHeight; };

    const std::vector<Spriteset>& getTileset() { return _tilesets; };
};

#endif
