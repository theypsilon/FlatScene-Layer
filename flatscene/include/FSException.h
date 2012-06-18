#ifndef __FS_EXCEPTION__
#define __FS_EXCEPTION__

#include <exception>
#include <string>
#include <memory>

namespace FlatScene {

    class Exception : public std::exception {
        struct ExceptionImpl;
        const std::unique_ptr<ExceptionImpl> _impl;
    public:
        Exception(std::string describe, int line = -1);
        Exception(const Exception&);
        virtual ~Exception() throw();
        virtual const char* what() const throw();
    };

} // flatscene

#endif