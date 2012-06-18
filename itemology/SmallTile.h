#ifndef I_TILE_SMALL_FS
#define I_TILE_SMALL_FS

#include "IInclude.h"

class SmallTile {
    typedef FS::U::IndexedPointer<unsigned short, const Sprite> SpritePointer;
    
    SpritePointer   _graph;
    SpritePointer   _collision;
    unsigned short  _flags;
public:
    SmallTile(const SmallTile& tile)
        : _graph(tile._graph), _collision(tile._collision), _flags(tile._flags)
    {}

    SmallTile& operator=(const SmallTile& rhs) {
        _graph      = rhs._graph;
        _collision  = rhs._collision;
        _flags      = rhs._flags;
        return *this;
    }

    SmallTile(const Sprite* const graph, const Sprite* const collision, unsigned short flags)
        : _graph(graph), _collision(collision), _flags(flags)
    {}

    const Sprite* const getGraph()     const { return _graph;     }
    const Sprite* const getCollision() const { return _collision; }
    unsigned short      getFlags()     const { return _flags;     }
};

#endif
