#ifndef FS_EXCEPTION__
#define FS_EXCEPTION__

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

    typedef Exception FileDoesntExistException;
    typedef Exception ExternalLibraryException;
    typedef Exception SDLException;
    typedef Exception ControlViolationException;

/* TODO FIXME

    class FileDoesntExistException : public Exception {};
    class ExternalLibraryException : public Exception {};
    class SDLExcetion : public ExternalLibraryException {};*/

} // flatscene

#endif