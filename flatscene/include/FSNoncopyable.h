#ifndef FS_NONCOPYABLE__
#define FS_NONCOPYABLE__

namespace FlatScene { namespace Util {

    class Noncopyable {
        protected:
            Noncopyable(){}
            ~Noncopyable(){}
        private:
            Noncopyable(const Noncopyable&);
            Noncopyable& operator=(const Noncopyable&);
    };

    class Nonmovable {
        protected:
            Nonmovable(){}
            ~Nonmovable(){}
        private:
            Nonmovable(Nonmovable&&);
            Nonmovable& operator=(Nonmovable&&);
    };

    class Noncpmvable : private Util::Noncopyable, private Nonmovable {};

}} // FlatScene::Util

#endif
