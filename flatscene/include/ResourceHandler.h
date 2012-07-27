#ifndef FS_RESOURCE_HANDLER__
#define FS_RESOURCE_HANDLER__

#include <memory>

namespace FlatScene {

	template <typename Resource>
	class ResourceHandler {
	public:
		ResourceHandler(const ResourceHandler& handler) 
			: _res(new Resource(*handler._res))
		{}

		ResourceHandler(ResourceHandler&& handler)
			: _res(std::move(handler._res))
		{}

		ResourceHandler& operator=(const ResourceHandler& rhs) {
			if (this != &rhs)
				this->_res.reset(new ResourceHandler(*rhs._res));
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

        template <typename ReturnResource = Resource> inline ReturnResource& getRes() const {
            return static_cast<ReturnResource&>(*_res);
        }
	private:
		std::unique_ptr<Resource> _res;
	};

} // FlatScene

namespace std {
    template<typename Resource> void swap(
    	FlatScene::ResourceHandler<Resource>& lhs, FlatScene::ResourceHandler<Resource>& rhs
    ) { lhs.swap(rhs); }
} // std

#endif