#ifndef __RASPLIB_EXCEPTION_HPP__
#define __RASPLIB_EXCEPTION_HPP__


#include <exception>
#include <string>


namespace rasplib {
namespace exception {




class Exception : public std::exception
{
private:
    int _code;
    std::string _message;
public:
    Exception(int code, std::string message) : _code(code), _message(message) {}

public:
    int code() {return _code;};
    const char* what() {return (const char*) _message.c_str();}

};




}
}

#endif
