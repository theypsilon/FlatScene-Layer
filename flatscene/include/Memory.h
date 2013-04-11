#ifndef FS_CACHE_H__
#define FS_CACHE_H__

#include <memory>
#include <unordered_map>
#include <cassert>

namespace FlatScene {

    template <typename K, typename V>
    class Cache {
        typedef std::unordered_map<K, std::weak_ptr<V>> KeyValMap;
        typedef std::unordered_map<V*, typename KeyValMap::iterator>       DeleteMap;

        KeyValMap _kvmap;
        DeleteMap _delmap;
    public:
        std::shared_ptr<V> get(const K& k) {
            if (auto it = _kvmap.find(k) != _kvmap.end())
                return _kvmap[k].lock();

            auto res = std::shared_ptr<V>(
                new V(k),
                [&] (V* p) {
                    assert(p);
                    auto   it  = _delmap.find(p);
                    assert(it != _delmap.end ());
                    assert(it->second.expired());
                    _kvmap .erase(it->second);
                    _delmap.erase(it);
                    delete p;
                }
            );
            auto delit = _kvmap.emplace(k,res).first;
            _delmap.emplace(res.get(), delit);
            return res;
        }
    };

    template <typename T, typename CacheIndex>
    std::shared_ptr<T> make_cached_shared(const CacheIndex& index) {
        static Cache<CacheIndex,T> cache;
        return cache.get(index);
    }

} // FlatScene

#endif // FS_CACHE_H__