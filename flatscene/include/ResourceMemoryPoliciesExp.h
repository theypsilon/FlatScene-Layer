#ifndef FS_RESOURCE_MEMORY_POLICIES
#define FS_RESOURCE_MEMORY_POLICIES

#include <unordered_map>
#include <functional>
#include <limits>
#include <cassert>

namespace FlatScene {

    template <typename Resource,typename HolderTemplate>
    struct ExplicitHolderPolicy {
        typedef Resource        ResourceType;
        typedef HolderTemplate  Holder;
    };

    template <typename Resource>
    struct PointerHolderPolicy {
        typedef Resource    ResourceType;
        typedef Resource*   Holder;
    };

    template <typename HolderPolicy>
    struct StandardMovePolicy {
        typedef typename HolderPolicy::Holder Holder;
        static inline Holder move(Holder& res) {
            return std::move(res);
        }
    };

    template <typename HolderPolicy>
    struct PointerAccessPolicy {
        typedef typename HolderPolicy::Holder       Holder;
        typedef typename HolderPolicy::ResourceType Resource;
        static inline Resource& access(Holder& res) {
            return *res;
        }
    };

    template <typename HolderPolicy>
    struct TrivialComparePolicy {
        typedef const typename HolderPolicy::Holder& Holder;
        static inline bool isSame(Holder lhs,Holder rhs) {
            return lhs == rhs;
        }
    };

    template <
        typename Resource, 
        typename HolderPolicy = ExplicitHolderPolicy<Resource,std::unique_ptr<Resource> > 
    >
    struct DefaultMemoryPolicy
    : public StandardMovePolicy     <HolderPolicy>
    , public PointerAccessPolicy    <HolderPolicy> {
        typedef typename HolderPolicy::Holder Holder;

        static bool isSame(Holder& lhs, Holder& rhs) {
            return false;
        }

        static Resource* add(const typename HolderPolicy::Holder& res) {
            return new Resource(*res);
        }

        static void remove(typename HolderPolicy::Holder& res) {
            res.reset(nullptr);
        }
    };

    template <
        typename Resource, 
        typename HolderPolicy = PointerHolderPolicy<Resource>, 
        typename CountIndex = unsigned int
    >
    class RefCountMemoryPolicy 
    : public PointerAccessPolicy    <HolderPolicy>
    , public TrivialComparePolicy   <HolderPolicy> {
    public:
        typedef Resource*           Holder;
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

        static inline const CountMap& getCounts() {
            return _count;
        }

    private:
        static void destroy(Holder res);

        static CountMap _count;
    };

    template <typename Resource,typename HolderPolicy, typename CountIndex>
    typename std::unordered_map<Resource*,CountIndex>
        RefCountMemoryPolicy<Resource,HolderPolicy,CountIndex>::_count;

} // FlatScene

#endif