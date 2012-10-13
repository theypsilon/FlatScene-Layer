#ifndef FS_RESOURCE_HANDLER__
#define FS_RESOURCE_HANDLER__

#include <memory>
#include <type_traits>
#include <iostream>

#include "ResourceMemoryPolicies.h"

namespace FlatScene {

    template < typename Resource, typename MemoryPolicy = DefaultMemoryPolicy<Resource>	> 
    class ResourceHandler : MemoryPolicy {
        typedef typename MemoryPolicy::Holder Holder;

        using MemoryPolicy::access;
        using MemoryPolicy::add;
        using MemoryPolicy::remove;
        using MemoryPolicy::move;
        using MemoryPolicy::isSame;
    public:
        typedef Resource        ResourceType;
        typedef MemoryPolicy    MemoryPolicyType;

        ResourceHandler(const ResourceHandler& handler) 
            : _res(add(handler._res))
        {}

        ResourceHandler(ResourceHandler&& handler)
            : _res(move(handler._res))
        {}

        virtual ~ResourceHandler() {
            remove(_res);
        }

        ResourceHandler& operator=(const ResourceHandler& rhs) {
            if (this != &rhs && !isSame(_res,rhs._res)) {
                remove(_res);
                _res = add(rhs._res);
            }
            return *this;
        }

        ResourceHandler& operator=(ResourceHandler&& rhs) {
            this->_res = move(rhs._res);
            return *this;
        }

        void swap(ResourceHandler& rhs) {
            auto aux = move(rhs._res);
            rhs._res = move(_res);
            _res = move(aux);
        }

    protected:
        ResourceHandler(Holder res) : _res(add(res)) {}

        template <typename R> inline R& getRes() const {
            return static_cast<R&>(access(_res));
        }

        inline Resource& getRes() const {
            return static_cast<Resource&>(access(_res));
        }
    private:
        Holder _res;
    };

    // class TestResourceHandler : ResourceHandler<int> { 
    //     public: TestResourceHandler(std::unique_ptr<int> n) : ResourceHandler(std::move(n)) {} 
    // };

} // FlatScene

namespace std {
    template<typename Resource> void swap(
        FlatScene::ResourceHandler<Resource>& lhs, FlatScene::ResourceHandler<Resource>& rhs
    ) { lhs.swap(rhs); }
} // std

#endif