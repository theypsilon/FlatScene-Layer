#ifndef FS_SINGLETON__
#define FS_SINGLETON__

namespace FlatScene { namespace Util {

    template <class T>
    class Singleton {
        public:
            static T& I() {
                static T instance;
                return instance;
            };
    };

}} // FlatScene::util

#endif // FS_SINGLETON__
