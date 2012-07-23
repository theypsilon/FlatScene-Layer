#ifndef FS_ALGORITHM__
#define FS_ALGORITHM__

#include <algorithm>

namespace FlatScene {

    namespace result_of { // pillaged from Boost ;)
        template <class Value>
        struct extract { typedef Value type; };

        template <class First, class Second>
        struct extract < std::pair<First,Second> > { typedef Second type; };
    } // result_of

    template <class Value>
    Value extract(Value v) { return v; }

    template <class First, class Second>
    Second extract(std::pair<First,Second> pair) { return pair.second; }

    template <class Container>
    typename result_of::extract< typename Container::value_type >::type
    first(const Container& c) { return extract(*c.begin()); }

    template <typename Container, typename Key, typename Value>
    Value find_assoc(Container& container, Key needle, Value default = Value()) {
        auto it = container.find(needle);

        if (end(container) == it)
            return default;

        return it->second;
    }
} // FlatScene

#endif // FS_ALGORITHM__