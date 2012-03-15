#ifndef __FS_NONCOPYABLE__
#define __FS_NONCOPYABLE__

namespace flatscene {

    class Noncopyable {
        protected:
            Noncopyable(){}
            ~Noncopyable(){}
        private:
            Noncopyable(const Noncopyable&);
            const Noncopyable& operator=(const Noncopyable&);
    };

} // flatscene

#endif
