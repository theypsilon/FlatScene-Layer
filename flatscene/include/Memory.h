#ifndef FS_CACHE_H__
#define FS_CACHE_H__

#include <memory>
#include <unordered_map>
#include <cassert>
#include <type_traits>

namespace FlatScene {

    template <typename K, typename V>
    class Cache {
        typedef std::unordered_map<K, std::weak_ptr<V>> KeyValMap;
        typedef std::unordered_map<V*, typename KeyValMap::iterator>       DeleteMap;

        template <typename T>
        using is_key_constructible = typename std::enable_if
            <std::is_constructible <V,const T&>  ::value>  ::type;

        KeyValMap _kvmap;
        DeleteMap _delmap;

        std::shared_ptr<V> intern_get(const K& k, std::function<V*()> factory) {
            if (auto it = _kvmap.find(k) != _kvmap.end())
                return _kvmap[k].lock();

            assert(factory);
            auto res = std::shared_ptr<V>(
                factory(),
                [&] (V* p) {
                    assert(p);
                    auto   it  = _delmap.find(p);
                    assert(it != _delmap.end ());
                    //assert(it->second.expired()); @TODO uncomment it when available in libstdc++
                    _kvmap .erase(it->second);
                    _delmap.erase(it);
                    delete p;
                }
            );
            auto delit = _kvmap.emplace(k,res).first;
            _delmap.emplace(res.get(), delit);
            return res;
        }

    public:
        bool has(const K& k) const {
            return _kvmap.find(k) != _kvmap.end();
        }

        std::size_t size() const {
            return _kvmap.size();
        }

        std::shared_ptr<V> get(const K& k, std::function<V*()> factory) {
            return intern_get(k,factory);
        }

        template <typename... Dummy, typename T = K>
        std::shared_ptr<V> get(const K& k, is_key_constructible<T>* = 0) {
            static_assert(sizeof...(Dummy) == 0, "Do not specify template arguments!");
            return intern_get(k,[&]{ return new V(k); });
        }

        static Cache<K,V>& instance() {
            static Cache<K,V> cache;
            return cache;
        }    

    };

    template <typename T, typename CacheIndex>
    std::shared_ptr<T> make_cached_shared(const CacheIndex& index) {
        return Cache<CacheIndex,T>::instance().get(index);
    }

    template <typename T, typename CacheIndex>
    std::shared_ptr<T> make_cached_shared(const CacheIndex& index, std::function<T*()> factory) {
        return Cache<CacheIndex,T>::instance().get(index, factory);
    }

    template <typename T>
    bool operator==(const std::weak_ptr<T>& lhs, const std::weak_ptr<T>& rhs) {
        auto s1 = lhs.lock();
        auto s2 = rhs.lock();
        return s1.get() == s2.get();
    }

} // FlatScene


namespace std {
    template <typename T> struct hash<std::weak_ptr<T>> {
        size_t operator()(const std::weak_ptr<T> & w) const {
            auto p = w.lock();
            return hash<T*>()(p.get());
        }
    };
}

#endif // FS_CACHE_H__