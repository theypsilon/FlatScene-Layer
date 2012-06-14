#ifndef I_TILE_SMALL_FS
#define I_TILE_SMALL_FS

#include "FSLibrary.h"
#include <unordered_map>
#include <vector>
#include <limits>
#include <algorithm>

class SmallTile {

    static std::unordered_map<unsigned int,const void*> _pointerMapper;
    static std::vector<unsigned short> _graphCount;
    static std::vector<unsigned short> _collisionCount;

    static unsigned int _storeSprite(const flatscene::Sprite* spt, bool graph = true) {
        std::vector
        <unsigned short>&   count = graph? _graphCount : _collisionCount;
        unsigned char       shift = graph? 0 : 16;
        unsigned int        index = count.size();

        if (index >= std::numeric_limits<unsigned short>::max()) {
            auto it = std::find(count.begin(),count.end(),[](unsigned short c){
                return c == 0;
            });
            if (it == count.end()) {
                ;
            }
            index = it - count.begin();
            *it++;
        } else {
            count.push_back(1);
        }

        _pointerMapper[index << shift] = reinterpret_cast<const void*>(spt);
        return index;
    } 

    static const flatscene::Sprite* _getSprite(unsigned int i, bool graph = true) {
        return reinterpret_cast<const flatscene::Sprite*>(_pointerMapper.at(graph ? i : i << 16));
    }

    void _increaseIndexCounter() const {
        _graphCount    [_graphIndex      ] ++;
        _collisionCount[_collisionIndex  ] ++;
    }

    void _releaseIndexCounter() const {
        _graphCount    [_graphIndex     ] --;
        _collisionCount[_collisionIndex ] --;
    }

    unsigned short _graphIndex;
    unsigned short _collisionIndex;
    unsigned short _flags;

public:

    SmallTile(const SmallTile& tile)
        : _flags(tile._flags), _graphIndex(tile._graphIndex), _collisionIndex(tile._collisionIndex)
    {
        _increaseIndexCounter();
    }

    SmallTile(const flatscene::Sprite* const graph, const flatscene::Sprite* const collision, unsigned short flags)
        : _flags(flags), _graphIndex(_storeSprite(graph)), _collisionIndex(_storeSprite(collision))
    {
        _increaseIndexCounter();
    }

    SmallTile& operator=(const SmallTile& rhs) {
        _releaseIndexCounter();
        _graphIndex     = rhs._graphIndex;
        _collisionIndex = rhs._collisionIndex;
        _flags          = rhs._flags;
        _increaseIndexCounter();
        return *this;
    }

    ~SmallTile() {
        _releaseIndexCounter();
    }

    const flatscene::Sprite* const getGraph() const {
        return _getSprite(_graphIndex);
    }
    const flatscene::Sprite* const getCollision() const {
        return _getSprite(_collisionIndex,false);
    }

    unsigned short getFlags() const { 
        return _flags;     
    }
};

#endif
