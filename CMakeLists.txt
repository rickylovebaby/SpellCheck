cmake_minimum_required(VERSION 3.7)
project(SpellCheck)

set(CMAKE_CXX_STANDARD 11)


set(SOURCE_FILES

        src/testmain.cpp)

include_directories(include)

add_executable(SpellCheck ${SOURCE_FILES})