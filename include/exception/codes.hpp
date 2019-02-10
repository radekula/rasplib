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



#ifndef __RASPLIB_EXCEPTION_CODES_HPP__
#define __RASPLIB_EXCEPTION_CODES_HPP__




#define INVALID_DEVICE                  1
#define DEVICE_INFO_ERROR               2
#define INVALID_PIN_REQUEST             10
#define REQUEST_ERROR                   51
#define LINE_WRITE_ERROR                52
#define LINE_READ_ERROR                 53
#define SIGNAL_MASK_CREATE_ERROR        101
#define SIGNAL_HANDLER_CREATE_ERROR     102
#define INTERRUPTS_HANDLER_NOT_EXISTS   201
#define INTERRUPT_NOT_FOUND             202
#define I2C_BUS_ERROR                   301
#define I2C_DEVICE_ERROR                302
#define FREQUENCY_NOT_SUPPORTED         401


#endif
