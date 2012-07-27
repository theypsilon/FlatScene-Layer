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

		void swap(const ResourceHandler& rhs) {
			ResourceHandler&& aux = std::move(rhs);
			rhs = std::move(*this);
			*this = std::move(aux);
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

#endif