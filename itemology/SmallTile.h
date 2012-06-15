#ifndef I_TILE_SMALL_FS
#define I_TILE_SMALL_FS

#include "FSLibrary.h"
#include <unordered_map>
#include <vector>
#include <limits>
#include <algorithm>
#include <cassert>

class SmallTile {

    typedef unsigned short                  IndexType;
    typedef std::numeric_limits<IndexType>  IndexLimit;
    typedef unsigned int                    DouleIndexType;

    static std::unordered_map<DouleIndexType,const void*> _pointerMapper;
    static std::unordered_map<const void*,DouleIndexType> _indexMapper;
    static std::vector<IndexType> _graphCount;
    static std::vector<IndexType> _collisionCount;


    static IndexType _storeSprite(const flatscene::Sprite* spt, bool graph = true) {
        std::vector<IndexType>&
                            count = graph? _graphCount : _collisionCount;
        const void*         ptr   = reinterpret_cast<const void*>(spt);
        unsigned char       shift = graph? 0 : sizeof(IndexType)*8;
        const std::size_t   size  = count.size();
        IndexType           index = _indexMapper.find(ptr) != _indexMapper.end()
                            ? _indexMapper[ptr] >> shift   : size;

        if (size == index) {
            count.push_back(1);
        } else {
            count[index]++;
        }

        assert(count.size() <= IndexLimit::max());

        _pointerMapper[index << shift] = ptr;
        return index;
    } 

    static const flatscene::Sprite* _getSprite(DouleIndexType i, bool graph = true) {
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
