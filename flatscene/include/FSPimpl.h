#ifndef __FS_PIMPL__
#define __FS_PIMPL__

#include <memory>

namespace flatscene {

    template <class T>
    class Pimpl {
    private:
        std::unique_ptr<T> m;
    public:
        Pimpl() : m(new T) {}
        /*
        template<typename ...Args> Pimpl(Args&&... args)
                : m(new T(std::forward(args)...)) {}
                */
        ~Pimpl() {}

        T* operator->() {
            return m.get();
        }

        T& operator*() {
            return *m.get();
        }
    };

} // flatscene

#endif
