#ifndef FS_RESOURCE_MEMORY_POLICIES_EXP
#define FS_RESOURCE_MEMORY_POLICIES_EXP

#include <unordered_map>
#include <functional>
#include <limits>
#include <cassert>

namespace FlatScene {

    template <typename Holder> struct StandardMovePolicy {
        inline Holder move(Holder& res) const { return std::move(res); }
    };

    template <typename Holder> struct PointerMovePolicy {
        inline Holder move(Holder& res) const { 
            auto out = res;
            res = nullptr;
            return out; 
        }
    };

    template <typename Holder> struct TrivialComparePolicy {
        inline bool isSame(Holder lhs,Holder rhs) const { return lhs == rhs; }
    };

    template <typename Holder, bool retIsSame> struct PredefinedComparePolicy {
        inline bool isSame(Holder& lhs,Holder& rhs) const { return retIsSame; }
    };

    template <typename Holder, typename Resource> struct PointerAccessPolicy {
        inline Resource& access(const Holder& res) const {
            return *res;
        }
    };

    template <typename Resource, typename H = std::unique_ptr<Resource> >
    class DefaultMemoryPolicy 
    : public PredefinedComparePolicy    <H,false>
    , public PointerAccessPolicy        <H,Resource>
    , public StandardMovePolicy         <H> {
    protected:
        typedef H Holder;

        using PointerAccessPolicy<H,Resource>::access;
        inline Holder add(const Holder& res) const { return Holder(new Resource(access(res))); }
        inline void remove(Holder& res) const {  res.reset(nullptr); }
    };

    template <
        typename Resource,
        typename H = Resource*,
        typename CountIndex = unsigned int
    >
    class RefCountMemoryPolicy
    : public PointerAccessPolicy    <H,Resource>
    , public TrivialComparePolicy   <H>
    , public PointerMovePolicy      <H> {
    public:
        typedef H                                       Holder;
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

    private:
        void destroy(Holder res) const;

        static CountMap _count;
    };

    template <typename Resource,typename H, typename CountIndex>
    typename std::unordered_map<H,CountIndex>
        RefCountMemoryPolicy<Resource,H,CountIndex>::_count;

    // In Impl file:
    // template <typename Resource, typename H, typename CountIndex>
    // void RefCountMemoryPolicy<Resource, H, CountIndex>::destroy(Holder res) const {
    //     delete res;
    // }

} // FlatScene

#endif // FS_RESOURCE_MEMORY_POLICIES_EXP