#ifndef I_INDEXED_POINTER_FS
#define I_INDEXED_POINTER_FS

#include <unordered_map>
#include <deque>
#include <limits>

#include "IndexHandler.h"

template <class IndexType, class PointerType>
class IndexedPointer {

    typedef std::numeric_limits<IndexType>  IndexLimit;
    typedef IndexedPointer<IndexType,PointerType> type;

    static std::unordered_map<IndexType,PointerType*>   _pointerMapper;
    static std::unordered_map<PointerType*,IndexType>   _indexMapper;
    static IndexHandler<IndexType>                      _indexCounter;

    static IndexType _storePointer(PointerType* ptr) {
        auto indexIt    = _indexMapper.find(ptr);
        IndexType index = (indexIt == _indexMapper.end())?
            _indexCounter.generateNew() : indexIt->second;

        _pointerMapper[index] = ptr;
        _indexMapper  [ptr  ] = index;
        return index;
    }

    static PointerType* _getPointer(IndexType index) {
        return _pointerMapper.at(index);
    }

    static void _increaseIndexCount(IndexType index) {
        _indexCounter.add(index);
    }

    static void _releaseIndexCount(IndexType index) {
        if (_indexCounter.remove(index)) {
            _indexMapper.erase(_pointerMapper[index]);
            _pointerMapper.erase(index);
        }
    }

    IndexedPointer(type&&);
    type& operator=(type&&);

    IndexType _index;

public:

    IndexedPointer(PointerType* ptr) 
        : _index(_storePointer(ptr)) 
    {
        _increaseIndexCount(_index);
    }

    IndexedPointer(const type& rhs)
        : _index(rhs._index)
    {
        _increaseIndexCount(_index);
    }

    type& operator=(const type& rhs) {
        if (&rhs == this)
            return *this;
        _releaseIndexCount(_index);
        _index = rhs._index;
        _increaseIndexCount(_index);
        return *this;
    }

    ~IndexedPointer() {
        _releaseIndexCount(_index);
    }

    PointerType& operator*() const {
        return *_getPointer(_index);
    }

    PointerType& operator->() const {
        return *_getPointer(_index);
    }
    
    operator PointerType*() const {
        return _getPointer(_index);
    }

    PointerType* get() const {
        return _getPointer(_index);
    }

    void swap(IndexedPointer<IndexType,PointerType>& rhs) {
        IndexType temp = _index;
        _index = rhs._index;
        rhs._index = temp;
    }
};

template <class IndexType, class PointerType>
void swap(IndexedPointer<IndexType,PointerType>& first, IndexedPointer<IndexType,PointerType>& second) {
    first.swap(second);
}

template <typename IndexType, typename PointerType>
std::unordered_map<IndexType,PointerType*>
IndexedPointer<IndexType,PointerType>::_pointerMapper;

template <typename IndexType, typename PointerType>
std::unordered_map<PointerType*,IndexType>
IndexedPointer<IndexType,PointerType>::_indexMapper;

template <typename IndexType, typename PointerType>
IndexHandler<IndexType>
IndexedPointer<IndexType,PointerType>::_indexCounter;

#endif
