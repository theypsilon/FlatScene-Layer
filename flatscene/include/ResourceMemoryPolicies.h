#ifndef FS_RESOURCE_MEMORY_POLICIES
#define FS_RESOURCE_MEMORY_POLICIES

#include <unordered_map>
#include <functional>
#include <limits>
#include <cassert>

namespace FlatScene {

    template <typename Resource>
    class DefaultMemoryPolicy {
    protected:
        typedef std::unique_ptr<Resource> Holder;

        inline bool isSame(Holder& lhs, Holder& rhs) const {
            return false;
        }

        inline Holder add(const Holder& res) const {
            return Holder(new Resource(access(res)));
        }

        inline void remove(Holder& res) const {
            res.reset(nullptr);
        }

        inline Holder move(Holder& res) const {
            return std::move(res);
        }

        inline Resource& access(const Holder& res) const {
            return *res;
        }
    };

    template <typename Resource, typename CountIndex = unsigned int>
    class RefCountMemoryPolicy {
    public:
        typedef Resource*                               Holder;
        typedef std::unordered_map<Holder,CountIndex>   CountMap;

        static inline const CountMap& getCounts() {
            return _count;
        }

    protected:
        inline Holder add(Holder res) const {
            if (res) {
                _count[res]++;
                assert(_count[res] != std::numeric_limits<CountIndex>::min());
            }
            return res;
        }

        inline void remove(Holder res) const {
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

        inline Holder move(Holder& res) const {
            Holder out = res;
            res = nullptr;
            return out;
        }

        inline Resource& access(const Holder& res) const {
            return *res;
        }

        inline bool isSame(Holder& lhs, Holder& rhs) {
            return lhs == rhs;
        }

    private:
        void destroy(Holder res) const;

        static CountMap _count;
    };

    template <typename Resource, typename CountIndex>
    typename std::unordered_map<Resource*,CountIndex>
        RefCountMemoryPolicy<Resource,CountIndex>::_count;

} // FlatScene

#endif