cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

# MSVC runtime library flags are selected by an abstraction. Supported by CMake 3.15 and up
IF (POLICY CMP0091)
    CMAKE_POLICY (SET CMP0091 NEW)
ENDIF ()

project(PyPoc)

SET(CMAKE_CXX_STANDARD 11)

FIND_PACKAGE(Python3 REQUIRED COMPONENTS Development Interpreter)
FIND_PACKAGE(Boost REQUIRED COMPONENTS python310)
include_directories(${Python3_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

add_executable(PyPoc main.cpp)
target_compile_definitions(PyPoc PUBLIC "BOOST_ALL_DYN_LINK" "$<$<CONFIG:Debug>:BOOST_DEBUG_PYTHON>" "$<$<CONFIG:Debug>:Py_DEBUG>")

target_link_libraries(PyPoc PRIVATE ${Python3_LIBRARIES} Boost::python310)
