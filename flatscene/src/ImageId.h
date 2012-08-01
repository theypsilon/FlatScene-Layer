#ifndef FS_IMAGE_ID__
#define FS_IMAGE_ID__

#include <string>

namespace FlatScene {

    class ImageId {
    public:
        typedef unsigned int    Index;
        typedef std::string     Ident;

        ImageId(Index n, Ident file) : _n(n), _file(std::move(file)) {}

        Index           getIndex() const { return _n;    }
        const Ident&    getFile () const { return _file; }

        bool            operator==(const ImageId& rhs) const {
            return _n == rhs._n && _file == rhs._file;
        }
    private:
        Index _n;
        Ident _file;
    };

} // FlatScene

#endif // FS_IMAGE_ID__