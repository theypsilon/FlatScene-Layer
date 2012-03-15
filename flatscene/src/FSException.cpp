#include "FSException.h"

namespace flatscene {

struct Exception::ExceptionImpl {
    std::string description;

    ExceptionImpl(std::string describe, int line = -1)
        : description( line > -1 ? describe : describe) {}

    ExceptionImpl(const ExceptionImpl& e)
        : description( e.description ) {}

    const char* what() const throw() {
        return description.c_str();
    }
};

Exception::Exception(std::string describe, int line) 
: _impl( new ExceptionImpl(describe,line) ) {}

Exception::Exception(const Exception& e) 
: _impl( new ExceptionImpl(*e._impl) ) {}

Exception::~Exception() throw() {}

const char* Exception::what() const throw() {
    return _impl->what();
}

} // flatscene
