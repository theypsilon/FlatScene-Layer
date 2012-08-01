#ifndef FS_EXCEPTION__
#define FS_EXCEPTION__

#include <exception>
#include <string>
#include <memory>

namespace FlatScene {

    class Exception : public std::exception {
    public:
        Exception(std::string describe, int line = -1)
            : _description(std::move(describe)) 
        {}

        virtual ~Exception() throw() {}

        virtual const char* what() {
            return _description.c_str();
        }
    private:
        std::string _description;
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