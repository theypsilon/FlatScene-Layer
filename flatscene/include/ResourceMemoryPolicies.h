#ifndef FS_RESOURCE_MEMORY_POLICIES
#define FS_RESOURCE_MEMORY_POLICIES

#include <unordered_map>
#include <functional>
#include <limits>
#include <cassert>

namespace FlatScene {

    template <typename Resource>
    struct DefaultMemoryPolicy {
        typedef std::unique_ptr<Resource> Holder;

        static inline bool isSame(Holder& lhs, Holder& rhs) {
            return false;
        }

        static inline Resource* add(const Holder& res) {
            return new Resource(access(res));
        }

        static inline void remove(Holder& res) {
            res.reset(nullptr);
        }

        static inline Holder move(Holder& res) {
            return std::move(res);
        }

        static inline Resource& access(const Holder& res) {
            return *res.get;
        }
    };

    template <typename Resource, typename CountIndex = unsigned int>
    class RefCountMemoryPolicy {
    public:
        typedef Resource*                               Holder;
        typedef std::unordered_map<Holder,CountIndex>   CountMap;

        static inline Holder add(Holder res) {
            if (res) {
                _count[res]++;
                assert(_count[res] != std::numeric_limits<CountIndex>::min());
            }
            return res;
        }

        static inline void remove(Holder res) {
            if (res) {
                auto it = _count.find(res);
                assert(it != _count.end());
                assert(it->second > 0);
                it->second--;
                if (it->second == 0) {
                    destroy(res);
                    _count.erase(res);
                }
            }
        }

        static inline Holder move(Holder& res) {
            Holder out = res;
            res = nullptr;
            return out;
        }

        static inline Resource& access(const Holder& res) {
            return *res;
        }

        static inline const CountMap& getCounts() {
            return _count;
        }

    private:
        static void destroy(Holder res);

        static CountMap _count;
    };

    template <typename Resource, typename CountIndex>
    typename std::unordered_map<Resource*,CountIndex>
        RefCountMemoryPolicy<Resource,CountIndex>::_count;

} // FlatScene

#endif