/*
    Rasplib - library for handling Raspberry Pi's GPIO and connected devices
    Copyright (C) 2018  Radosław Ulatowski

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/




#ifndef __RASPLIB_EXCEPTION_HPP__
#define __RASPLIB_EXCEPTION_HPP__


#include <exception>
#include <string>
#include <exception/codes.hpp>



namespace rasplib {



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

#endif
