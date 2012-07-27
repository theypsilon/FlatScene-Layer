#ifndef FS_RESOURCE_HANDLER__
#define FS_RESOURCE_HANDLER__

#include <memory>
#include <type_traits>

namespace FlatScene {

	template <typename Resource>
	struct DefaultMemoryPolicy {
		typedef std::unique_ptr<Resource> Holder;
		static bool isSame(Holder lhs, Holder rhs) {
			return false;
		}

		static Resource* clone(const Holder& res) {
			return new Resource(*res);
		}

		static void destroy(Holder& res) {
			res.reset(nullptr);
		}

		static void reset(Holder& res,const Holder& newval) {
			res.reset(new Resource(*newval));
		}
	};

	template < typename Resource, typename MemoryPolicy = DefaultMemoryPolicy<Resource>	> 
	class ResourceHandler {
	public:
		// ResourceHandler(const ResourceHandler& handler) 
		// 	: _res(new Resource(*handler._res))
		// {}
		ResourceHandler(const ResourceHandler& handler) 
			: _res(MemoryPolicy::clone(handler._res))
		{}

		ResourceHandler(ResourceHandler&& handler)
			: _res(std::move(handler._res))
		{}

		~ResourceHandler() {
			MemoryPolicy::destroy(_res);
		}

		// ResourceHandler& operator=(const ResourceHandler& rhs) {
		// 	if (this != &rhs && &getRes() != &rhs.getRes())
		// 		this->_res.reset(new ResourceHandler(*rhs._res));
		// 	return *this;
		// }

		ResourceHandler& operator=(const ResourceHandler& rhs) {
			if (this != &rhs && !MemoryPolicy::isSame(_res,rhs._res))
				MemoryPolicy::reset(_res);
			return *this;
		}

		ResourceHandler& operator=(ResourceHandler&& rhs) {
			this->_res = std::move(rhs._res);
			return *this;
		}

		void swap(ResourceHandler& rhs) {
			auto aux = std::move(rhs._res);
			rhs._res = std::move(_res);
			_res = std::move(aux);
		}

	protected:
		ResourceHandler(Resource* res) : _res(res) {}

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