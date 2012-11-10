#ifndef FS_SHARED_DATA_HANDLER__
#define FS_SHARED_DATA_HANDLER__

#include <memory>

namespace FlatScene { namespace Util {

    template <class DataType>
    class SharedDataHandler {
    protected:
        std::shared_ptr<DataType> _data;
    public:
        SharedDataHandler(DataType* ptr) 
            : _data{ptr}
        {}

        SharedDataHandler(const SharedDataHandler& rhs) 
            : _data{rhs._data}
        {}

        SharedDataHandler(SharedDataHandler&& rhs)
            : _data{std::move(rhs._data)}
        {}
        
        SharedDataHandler& operator=(const SharedDataHandler& rhs) {
            if (&rhs == this) return *this;

            this._data = rhs._data;
            return *this;
        }

        SharedDataHandler& operator=(SharedDataHandler&& rhs) {
            if (&rhs == this) return *this;

            this._data = std::move(rhs._data);
            rhs._data.reset(nullptr);
            return *this;
        }

    };

}} // FlatScene::Util

#endif // FS_SHARED_DATA_HANDLER__


