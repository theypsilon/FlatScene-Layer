#ifndef I_INDEXED_POINTER_FS
#define I_INDEXED_POINTER_FS

#include <unordered_map>
#include <map>
#include <deque>
#include <limits>
#include <algorithm>

#include "IndexHandler.h"

namespace FlatScene { namespace Util {

    template <class IndexType, class PointerType, bool CreationBoost=true, int Instance=0>
    class IndexedPointer {

        typedef IndexedPointer<IndexType,PointerType,CreationBoost,Instance>    Type;
        typedef std::numeric_limits<IndexType>                                  IndexLimit;

        static typename std::enable_if<CreationBoost,
            std::unordered_map<PointerType*,IndexType> >::type  _indexMap;
        static std::unordered_map<IndexType,PointerType*>       _ptrMap;
        static IndexHandler<IndexType>                          _indexCounter;

        IndexType _storePointer(PointerType* ptr) {
            if (ptr) {
                //static if (CreationBoost) { //@TODO Optimization for C++11 compilers
                auto it = _indexMap.find(ptr);
                //} else {
                //    auto it = std::find_if(_ptrMap.begin(),_ptrMap.end(),[&](const decltype(*_ptrMap.end())& value) {
                //        return value.second == ptr; 
                //    });
                //}

                IndexType index = (it == _indexMap.end())? _indexCounter.generateNew() : it->second;

                _ptrMap  [index] = ptr;
                //static if (CreationBoost) {
                _indexMap[ptr  ] = index;
                //}
                return index;
            } else {
                return _indexCounter.getInvalid();
            }
        }

        PointerType* _getPointer() const {
            if (!operator bool()) {
                throw Exception("This indexed pointer has been moved and now is empty.");
            }
            return _ptrMap.at(_index);
        }

        void _increaseIndexCount() {
            if (operator bool())
                _indexCounter.add(_index);
        }

        void _releaseIndexCount() {
            if (operator bool() && _indexCounter.remove(_index)) {
                _indexMap.erase(_ptrMap[_index]);
                _ptrMap.erase(_index);
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

        IndexedPointer(const Type& rhs)
            : _index(rhs._index)
        {
            _increaseIndexCount();
        }

        IndexedPointer(Type&& rhs) 
            : _index(rhs._index) 
        {
            rhs._index = _indexCounter.getInvalid();
        }

        Type& operator=(const Type& rhs) {
            if (&rhs == this)
                return *this;

            _releaseIndexCount();
            _index = rhs._index;
            _increaseIndexCount();

            return *this;
        }

        Type& operator=(Type&& rhs) {
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

        void swap(Type& rhs) {
            IndexType temp = _index;
            _index = rhs._index;
            rhs._index = temp;
        }

        void reset(PointerType* ptr = nullptr) {
            *this = type(ptr);
        }
    };

    template <typename IndexType, typename PointerType,bool CreationBoost, int Instance>
    void swap(
        IndexedPointer<IndexType,PointerType,CreationBoost,Instance>& first,
        IndexedPointer<IndexType,PointerType,CreationBoost,Instance>& second)
    {
        first.swap(second);
    }

    template <typename IndexType, typename PointerType,bool CreationBoost, int Instance>
    std::unordered_map<IndexType,PointerType*>
    IndexedPointer<IndexType,PointerType,CreationBoost,Instance>::_ptrMap;

    template <typename IndexType, typename PointerType,bool CreationBoost, int Instance>
    typename std::enable_if<CreationBoost,std::unordered_map<PointerType*,IndexType> >::type
    IndexedPointer<IndexType,PointerType,CreationBoost,Instance>::_indexMap;

    template <typename IndexType, typename PointerType,bool CreationBoost, int Instance>
    IndexHandler<IndexType>
    IndexedPointer<IndexType,PointerType,CreationBoost,Instance>::_indexCounter;

}} // FlatScene::util

#endif // I_INDEXED_POINTER_FS
