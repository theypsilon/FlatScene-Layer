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

    template <typename Iterable,typename Value>
    inline auto find(const Iterable& c, const Value& val) -> decltype(end(c)) {
        using namespace std;
        return find(begin(c),end(c),val);
    }

    template <typename Iterable,typename Value>
    inline bool binary_search(const Iterable& c, const Value& val) {
        using namespace std;
        return find(begin(c),end(c),val) != end(c);
    }

    template <typename Container>
    inline typename Container::mapped_type 
    find_assoc(
        Container& container, 
        typename Container::key_type needle, 
        typename Container::mapped_type def = typename Container::mapped_type()
    ) {
        auto it = container.find(needle);

        if (end(container) == it)
            return def;

        return extract(*it);
    }

    template <class Container>
    inline void call_to_all( const Container& container ) {
        for (const auto& callback : container) {
            callback();
        }
    }

} // FlatScene

#endif // FS_ALGORITHM__