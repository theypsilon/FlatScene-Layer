#ifndef FS_TRAITS__
#define FS_TRAITS__

#include <cassert>
#include <type_traits>

namespace FlatScene {

    template <typename T>
    struct to_cref {
    private:
        typedef typename std::decay<
            typename std::conditional<std::is_pointer<T>::value,
                typename std::remove_pointer<T>::type, T
            >::type 
        >::type npcvType;
    public:
        typedef const npcvType&         type;
        typedef const npcvType*const    pointer;

        static inline type from(type v) {
            return v;
        }

        static inline type from(pointer p) {
            assert(p);
            return *p;
        }
    };

}

#endif // FS_TRAITS__