#ifndef I_TILE_NORMAL_FS
#define I_TILE_NORMAL_FS

#include "IInclude.h"

class Tile {
    const FlatScene::Sprite* const _graph;
    const FlatScene::Sprite* const _collision;
    unsigned short      _flags;
public:
    Tile(const Tile& tile)
        : _graph(tile._graph), _collision(tile._collision), _flags(tile._flags)
    {}

    Tile& operator=(const Tile& rhs) {
        return *this;
    }

    Tile(const FlatScene::Sprite* const graph, const FlatScene::Sprite* const collision, unsigned short flags)
        : _graph(graph), _collision(collision), _flags(flags)
    {}

    const FlatScene::Sprite* const getGraph()     const { return _graph;     }
    const FlatScene::Sprite* const getCollision() const { return _collision; }
    unsigned short                 getFlags()     const { return _flags;     }
};

#endif // I_TILE_NORMAL_FS
