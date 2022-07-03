cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

# MSVC runtime library flags are selected by an abstraction. Supported by CMake 3.15 and up
IF (POLICY CMP0091)
    CMAKE_POLICY (SET CMP0091 NEW)
ENDIF ()

project(PyPoc)

SET(CMAKE_CXX_STANDARD 11)

add_compile_definitions("BOOST_PYTHON_NO_PY_SIGNATURES" "BOOST_ALL_DYN_LINK" "$<$<CONFIG:Debug>:BOOST_DEBUG_PYTHON>" "$<$<CXX_COMPILER_ID:MSVC>:__x86_64__>")
add_compile_options("$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Debug>>:/Z7>")
add_link_options("$<$<CXX_COMPILER_ID:MSVC>:/DEBUG>")

FIND_PACKAGE(Python3 REQUIRED COMPONENTS Development)
SET(BOOST_PYTHON_COMPONENT "python${Python3_VERSION_MAJOR}${Python3_VERSION_MINOR}")
FIND_PACKAGE(Boost REQUIRED COMPONENTS ${BOOST_PYTHON_COMPONENT} )
SET(BOOST_PYTHON_NO_PY_SIGNATURES 1)
include_directories(${Python3_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

add_link_options("$<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:-Bsymbolic>")

add_executable(PyPoc main.cpp)

target_link_libraries(PyPoc PRIVATE ${Python3_LIBRARIES} ${Boost_LIBRARIES})
