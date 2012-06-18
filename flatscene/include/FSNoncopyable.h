#ifndef FS_NONCOPYABLE__
#define FS_NONCOPYABLE__

namespace FlatScene { namespace Util {

    class NonCopyable {
        protected:
            NonCopyable(){}
            ~NonCopyable(){}
        private:
            NonCopyable(const NonCopyable&);
            NonCopyable& operator=(const NonCopyable&);
    };

    class NonMovable {
        protected:
            NonMovable(){}
            ~NonMovable(){}
        private:
            NonMovable(NonMovable&&);
            NonMovable& operator=(NonMovable&&);
    };

    class NonCpmvable : private NonCopyable, private NonMovable {};

}} // FlatScene::Util

#endif
