#ifndef FS_EXCEPTION__
#define FS_EXCEPTION__

#include <exception>
#include <string>
#include <memory>

namespace FlatScene {

    class Exception : public std::exception {
    public:
        Exception(std::string describe = "", int line = -1, const char* file = nullptr)
            : _description(std::move(describe)) 
        {
            if (line > -1) {
                _description += " ::line = " + std::to_string(line);
            }
            if (file) {
                _description += " ::file = " + std::string(file);
            }
        }

        virtual ~Exception() throw() {}

        virtual const char* what() {
            return _description.c_str();
        }
    private:
        std::string _description;
    };

    template <unsigned int N>
    class DerivedException : public Exception {
    public:
        DerivedException(std::string describe = "", int line = -1, const char* file = nullptr)
            : Exception(std::move(describe) + " ::type = "+typeid(DerivedException<N>).name(),line,file)
        {}

        virtual ~DerivedException() throw() {}
    };

    typedef DerivedException<0> FileDoesntExistException;
    typedef DerivedException<1> ExternalLibraryException;
    typedef DerivedException<2> SDLException;
    typedef DerivedException<3> ControlViolationException;

} // flatscene

#endif