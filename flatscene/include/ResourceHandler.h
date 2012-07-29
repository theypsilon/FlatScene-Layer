#ifndef FS_RESOURCE_HANDLER__
#define FS_RESOURCE_HANDLER__

#include <memory>
#include <type_traits>

#include "ResourceMemoryPolicies.h"

namespace FlatScene {

    template < typename Resource, typename MemoryPolicy = DefaultMemoryPolicy<Resource>	> 
    class ResourceHandler {
    public:
        typedef Resource ResourceType;

        ResourceHandler(const ResourceHandler& handler) 
            : _res(MemoryPolicy::add(handler._res))
        {}

        ResourceHandler(ResourceHandler&& handler)
            : _res(MemoryPolicy::move(handler._res))
        {}

        ~ResourceHandler() {
            MemoryPolicy::remove(_res);
        }

        ResourceHandler& operator=(const ResourceHandler& rhs) {
            if (this != &rhs && !MemoryPolicy::isSame(_res,rhs._res)) {
                MemoryPolicy::remove(_res);
                _res = MemoryPolicy::add(rhs._res);
            }
            return *this;
        }

        ResourceHandler& operator=(ResourceHandler&& rhs) {
            this->_res = MemoryPolicy::move(rhs._res);
            return *this;
        }

        void swap(ResourceHandler& rhs) {
            auto aux = MemoryPolicy::move(rhs._res);
            rhs._res = MemoryPolicy::move(_res);
            _res = MemoryPolicy::move(aux);
        }

    protected:
        ResourceHandler(Resource* res) : _res(MemoryPolicy::add(res)) {}

        template <typename ReturnResource> inline ReturnResource& getRes() const {
            return static_cast<ReturnResource&>(*_res);
        }

        inline Resource& getRes() const {
            return static_cast<Resource&>(*_res);
        }
    private:
        typename MemoryPolicy::Holder _res;
    };

} // FlatScene

namespace std {
    template<typename Resource> void swap(
        FlatScene::ResourceHandler<Resource>& lhs, FlatScene::ResourceHandler<Resource>& rhs
    ) { lhs.swap(rhs); }
} // std

#endif