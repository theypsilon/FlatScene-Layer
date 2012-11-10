#ifndef I_INDEX_HANDLER_FS
#define I_INDEX_HANDLER_FS

#include <unordered_map>
#include <deque>
#include <limits>
#include <cassert>

#include "Exception.h"
#include "Noncopyable.h"

namespace FlatScene { namespace Util {

    template <typename IndexType, typename CounterType = unsigned int>
    class IndexHandler : public NonCpmvable {
        typedef IndexHandler<IndexType,CounterType> type;
        typedef std::numeric_limits<IndexType>      IndexLimit;
        typedef std::numeric_limits<CounterType>    CounterLimit;

        std::unordered_map<IndexType,CounterType>   _counters;
        std::deque<IndexType>                       _freeStack;
        IndexType                                   _nextIndex;

        auto _findCountIterator(IndexType index) -> decltype(_counters.end()) {
            auto it = _counters.find(index);
            if (_counters.end() == it)
                throw FlatScene::Exception(std::string("Index '")+std::to_string(index)+"' not found.");

            return it;
        }

    public:

        static IndexType getInvalid() {
            return IndexLimit::max();
        }

        IndexHandler() : _nextIndex{IndexLimit::min()} {}
        ~IndexHandler() {}

        bool remove(IndexType index) {
            assert(index != getInvalid());
            auto it = _findCountIterator(index);

            it->second--;

            if(CounterLimit::min() == it->second ) {
                _counters.erase(it);
                _freeStack.push_back(index);
                return true;
            }
            return false;
        }

        IndexType generateNew() {
            IndexType index;
            if (_freeStack.empty()) {
                if (IndexLimit::max() == _nextIndex) 
                    throw Exception("No more indexes available.");

                index = _nextIndex;
                _nextIndex++;
            } else {
                index = _freeStack.back();
                _freeStack.pop_back();
            }

            _counters[index] = CounterLimit::min();

            return index;
        }

        void add(IndexType index) {
            assert(index != getInvalid());
            auto it = _findCountIterator(index);
            it->second++;
        }

        CounterType getCount(IndexType index) {
            assert(index != getInvalid());
            auto it = _findCountIterator(index);
            return it->second;
        }

    };

    template <typename IndexType, typename CounterType>
    IndexType generateAndCountIndex(IndexHandler<IndexType,CounterType>& handler) {
        IndexType index = handler.generateNew();
        handler.add(index);
        return index;
    }

}} // FlatScene::util

#endif // I_INDEX_HANDLER_FS
