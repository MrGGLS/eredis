# project
cmake_minimum_required(VERSION 3.21)
cmake_policy(SET CMP0091 NEW)
project(easy-redis)
set(CMAKE_CXX_STANDARD 20)

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    link_libraries(ws2_32 wsock32)
    add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)
endif ()
# target
ADD_EXECUTABLE(easy-redis
        src/eredis.cpp
        src/dbops.cpp
        src/erobject.cpp
        src/controller.cpp
        src/Parser.cpp
        src/main.cpp
        src/controller.h
        src/dbdata.h
        src/dbops.h
        src/erdb.hpp
        src/eredis.hpp
        src/erobject.h
        src/Parser.h
        src/erdb.cpp
        src/utils.hpp
        src/utils.cpp
        src/json.hpp
        )