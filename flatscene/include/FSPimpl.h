#ifndef __FS_PIMPL__
#define __FS_PIMPL__

#include <memory>

template <class T>
class FSPimpl {
private:
    std::unique_ptr<T> m;
public:
    FSPimpl() : m(new T) {}

    template<typename ...Args> FSPimpl(Args&&... args)
            : m(new T(std::forward(args)...)) {}

    ~FSPimpl() {}

    T* operator->() {
        return m.get();
    }

    T& operator*() {
        return *m.get();
    }
};


#endif
