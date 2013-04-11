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

    private:
        Index _n;
        Ident _file;
    };

    inline bool operator==(const ImageId& lhs, const ImageId& rhs) {
        return lhs.getIndex() == rhs.getIndex() && 
               lhs.getFile () == rhs.getFile ();
    }

} // FlatScene

namespace {
    template <class T>
    inline void hash_combine(std::size_t& seed, const T& v) {
        std::hash<T> hasher;
        seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
    }
}

namespace std {
    template <> struct hash<FlatScene::ImageId> {
        size_t operator()(const FlatScene::ImageId & x) const {
            std::size_t  seed = 0;
            hash_combine(seed, x.getIndex());
            hash_combine(seed, x.getFile ());
            return seed;
        }
    };
}

#endif // FS_IMAGE_ID__