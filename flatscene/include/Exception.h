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
        {
            if (line > -1) {
                _description += "::line = " + std::to_string(line);
            }
        }

        virtual ~Exception() throw() {}

        virtual const char* what() {
            return _description.c_str();
        }
    private:
        std::string _description;
    };

    template <typename T>
    class DerivedException : public Exception {
    public:
        DerivedException(std::string describe, int line = -1)
            : Exception(std::move(describe) + " ::type = "+typeid(T).name(),line)
        {}

        virtual ~DerivedException() throw() {}
    };

    #define FS_DEF_EXCEPTION(EXCEPTION_NAME) /*struct EXCEPTION_NAME##Type {};*/ \
        typedef Exception EXCEPTION_NAME;

    FS_DEF_EXCEPTION(FileDoesntExistException);
    FS_DEF_EXCEPTION(ExternalLibraryException);
    FS_DEF_EXCEPTION(SDLException);
    FS_DEF_EXCEPTION(ControlViolationException);

} // flatscene

#endif