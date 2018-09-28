cmake_minimum_required (VERSION 3.0)
project (rasplib)

set (rasplib_VERSION_MAJOR 0)
set (rasplib_VERSION_MINOR 1)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

if (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 3.0)
    if (NOT DEFINED CMAKE_CXX_STANDARD)
        set(CMAKE_CXX_STANDARD 14)
    endif ()
else ()
    set (CMAKE_CXX_FLAGS "-std=c++14")
endif ()

set (BUILD_DIR "build")

include_directories (${CMAKE_CURRENT_LIST_DIR}/include)

file (GLOB LIB_RASPLIB_SRC "src/gpio/*.cxx" "src/display/*.cxx" "src/i2c/*.cxx")
add_library (rasplib SHARED ${LIB_RASPLIB_SRC})
add_library (rasplib_static STATIC ${LIB_RASPLIB_SRC})