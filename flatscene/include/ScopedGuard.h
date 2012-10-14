#ifndef FS_SCOPED_POINTER__
#define FS_SCOPED_POINTER__

#include <functional>
#include <iostream>

#include "Noncopyable.h"

namespace FlatScene {

    class ScopedGuard : private Util::NonCpmvable {
    public:

        ScopedGuard(std::function<void(void)> f) : _f(std::move(f)), _dismissed(false) {}

        ~ScopedGuard() { 
            if (!_dismissed) {
                execute();
            }
        }

        void dismiss() {
            _dismissed = true;
        }

    private:
        void execute() {
            using namespace std;
            try {
                _f();
            } catch (exception& e) {
                cerr << "ScopedGuard catched an exception: " 
                     << typeid(e).name() << " -> " << e.what() << endl;
            } catch(...) {
                cerr << "ScopedGuard catched an unknown throw" << endl;
            }
        }
        bool _dismissed;
        std::function<void(void)> _f;
    };
} // FlatScene

#endif
