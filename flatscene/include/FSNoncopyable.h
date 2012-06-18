#ifndef __FS_NONCOPYABLE__
#define __FS_NONCOPYABLE__

namespace flatscene {

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
