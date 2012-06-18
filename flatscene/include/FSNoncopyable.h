#ifndef FS_NONCOPYABLE__
#define FS_NONCOPYABLE__

namespace FlatScene {

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

    class Noncpmvable : private Noncopyable, private Nonmovable {};

} // flatscene

#endif
