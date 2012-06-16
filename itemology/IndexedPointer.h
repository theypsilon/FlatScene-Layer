#ifndef I_INDEXED_POINTER_FS
#define I_INDEXED_POINTER_FS

#include <unordered_map>
#include <map>
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

    IndexType _storePointer(PointerType* ptr) {
        if (ptr) {
            auto indexIt    = _indexMapper.find(ptr);
            IndexType index = (indexIt == _indexMapper.end())?
                _indexCounter.generateNew() : indexIt->second;

            _pointerMapper[index] = ptr;
            _indexMapper  [ptr  ] = index;
            return index;
        } else {
            return _indexCounter.getInvalid();
        }
    }

    PointerType* _getPointer() const {
        if (!operator bool()) {
            throw Exception("This indexed pointer has been moved and now is empty.");
        }
        return _pointerMapper.at(_index);
    }

    void _increaseIndexCount() {
        if (operator bool())
            _indexCounter.add(_index);
    }

    void _releaseIndexCount() {
        if (operator bool() && _indexCounter.remove(_index)) {
            _indexMapper.erase(_pointerMapper[_index]);
            _pointerMapper.erase(_index);
        }
    }

    IndexType _index;

public:

    IndexedPointer(PointerType* ptr) 
        : _index(_storePointer(ptr)) 
    {
        static_assert(std::is_convertible<IndexType,long long>::value,"Index type invalid for IndexedPointer.");
        _increaseIndexCount();
    }

    IndexedPointer(const type& rhs)
        : _index(rhs._index)
    {
        _increaseIndexCount();
    }

    IndexedPointer(type&& rhs) 
        : _index(rhs._index) 
    {
        rhs._index = _indexCounter.getInvalid();
    }

    type& operator=(const type& rhs) {
        if (&rhs == this)
            return *this;

        _releaseIndexCount();
        _index = rhs._index;
        _increaseIndexCount();

        return *this;
    }

    type& operator=(type&& rhs) {
        if (&rhs == this)
            return *this;

        _index = rhs._index;
        rhs._index = _indexCounter.getInvalid();

        return *this;
    }

    ~IndexedPointer() {
        _releaseIndexCount();
    }

    PointerType& operator*() const {
        return *_getPointer();
    }

    PointerType& operator->() const {
        return *_getPointer();
    }
    
    operator PointerType*() const {
        return _getPointer();
    }

    PointerType* get() const {
        return _getPointer();
    }

    operator bool() const {
        return _indexCounter.getInvalid() != _index;
    }

    void swap(IndexedPointer<IndexType,PointerType>& rhs) {
        IndexType temp = _index;
        _index = rhs._index;
        rhs._index = temp;
    }

    void reset(PointerType* ptr = nullptr) {
        *this = type(ptr);
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
