#ifndef I_INDEX_COUNTER_FS
#define I_INDEX_COUNTER_FS

#include <unordered_map>
#include <deque>
#include <limits>
#include <cassert>

template <typename IndexType, typename CounterType = unsigned int>
class IndexHandler {
    typedef IndexHandler<IndexType,CounterType> type;
    typedef std::numeric_limits<IndexType>      IndexLimit;
    typedef std::numeric_limits<CounterType>    CounterLimit;

    std::unordered_map<IndexType,CounterType>   _counters;
    std::deque<IndexType>                       _freeStack;
    IndexType                                   _nextIndex;

    IndexHandler(const type&);
    IndexHandler(type&&);
    type& operator=(const type&);
    type& operator=(type&&);

    auto _findCountIterator(IndexType index) -> decltype(_counters.end()) {
        auto it = _counters.find(index);
        if (_counters.end() == it)
            throw flatscene::Exception(std::string("index '")+std::to_string(index)+"' not found");

        return it;
    }

public:

    static IndexType getInvalid() {
        return IndexLimit::max();
    }

    IndexHandler() : _nextIndex(IndexLimit::min()) {}
    ~IndexHandler() {}

    bool remove(IndexType index) {
        assert(index != getInvalid());
        auto it = _findCountIterator(index);

        if(CounterLimit::min() == (-- it->second)) {
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
                throw Exception("no more indexes available");

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

#endif