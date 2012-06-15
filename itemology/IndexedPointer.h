#ifndef I_INDEXED_POINTER_FS
#define I_INDEXED_POINTER_FS

#include <unordered_map>
#include <stack>
#include <limits>

template <class IndexType, class PointerType>
class IndexedPointer {

    typedef std::numeric_limits<IndexType>  IndexLimit;
    typedef IndexedPointer<IndexType,PointerType> type;

    static std::unordered_map<IndexType,PointerType*>   _pointerMapper;
    static std::unordered_map<PointerType*,IndexType>   _indexMapper;
    static std::unordered_map<IndexType,IndexType>      _indexCounter;
    static std::stack<IndexType>                        _indexFreeStack;

    static IndexType _getNewIndex() {
        if (_indexFreeStack.empty()) {
            auto end = _pointerMapper.end();
            for (IndexType i = IndexLimit::min(); i <= IndexLimit::max(); i++)
                if (_pointerMapper.find(i) != end)
                    return i;

            throw std::exception("too many indexed things by this IndexedPointer");
        }

        return _indexFreeStack.pop();
    }

    static IndexType _storePointer(PointerType* ptr) {
        auto indexIt    = _indexMapper.find(ptr);
        IndexType index = indexIt == _indexMapper.end()? _getNewIndex() : *indexIt;

        _pointerMapper[index] = ptr;
        _indexMapper  [ptr  ] = index;
    }

    static PointerType* _getPointer(IndexType index) {
        return _pointerMapper.at(index);
    }

    static void _increaseIndexCount(IndexType index) {
        _indexCounter[index] ++;   
    }

    static void _releaseIndexCount(IndexType index) {
        auto counter = _indexCounter[index]--;
        if (0 == counter) {
            _indexMapper.erase(_pointerMapper[index]);
            _pointerMapper.erase(index);
            _indexCounter.erase(index);
            _indexFreeStack.push(index);
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

    IndexedPointer(const type& old)
        : _index(old._index)
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

    PointerType& operator*() {
        return *_getPointer(_index);
    }

    PointerType& operator->() {
        return *_getPointer(_index);
    }

    PointerType* get() {
        return _getPointer(_index);
    }
};

#endif
