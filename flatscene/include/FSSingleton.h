#ifndef FS_SINGLETON__
#define FS_SINGLETON__

namespace flatscene { namespace util {

    template <class T>
    class Singleton {
        public:
            static T& I() {
                static T instance;
                return instance;
            };
    };

}} // flatscene::util

#endif // FS_SINGLETON__
