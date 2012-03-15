#ifndef __FS_SINGLETON__
#define __FS_SINGLETON__

namespace flatscene {

    template <class T>
    class Singleton {
        public:
            static T& I() {
                static T instance;
                return instance;
            };
    };

} // flatscene

#endif
