#ifndef __FS_EXCEPTION__
#define __FS_EXCEPTION__

#include <exception>
#include <string>
#include <memory>

class FSException : public std::exception {
    struct ExceptionImpl;
    const std::unique_ptr<ExceptionImpl> _impl;
public:
    FSException(std::string describe, int line = -1);
    FSException(const FSException&);
    virtual ~FSException() throw();
    virtual const char* what() const throw();
};


#endif