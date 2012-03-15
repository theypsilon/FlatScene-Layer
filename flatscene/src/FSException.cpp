#include "FSException.h"

namespace flatscene {

struct FSException::ExceptionImpl {
    std::string description;

    ExceptionImpl(std::string describe, int line = -1)
        : description( line > -1 ? describe : describe) {}

    ExceptionImpl(const ExceptionImpl& e)
        : description( e.description ) {}

    const char* what() const throw() {
        return description.c_str();
    }
};

FSException::FSException(std::string describe, int line) 
: _impl( new ExceptionImpl(describe,line) ) {}

FSException::FSException(const FSException& e) 
: _impl( new ExceptionImpl(*e._impl) ) {}

FSException::~FSException() throw() {}

const char* FSException::what() const throw() {
    return _impl->what();
}

} // flatscene
