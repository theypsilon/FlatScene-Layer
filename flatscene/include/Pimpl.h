#ifndef FS_PIMPL__
#define FS_PIMPL__

#include <memory>

namespace FlatScene { namespace Util {

    template <class T>
    class Pimpl {
    private:
        std::unique_ptr<T> m;
    public:
        Pimpl(Pimpl && p) : m{std::move(p.m)} {}

        template<typename ...Args> 
        Pimpl(Args&&... args) : m{new T(std::forward(args)...)} {}
                
        ~Pimpl() {}

        T* operator->() {
            return m.get();
        }

        T& operator*() {
            return *m.get();
        }
    };

}} // FlatScene::Util

#endif
