cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

## MSVC runtime library flags are selected by an abstraction. Supported by CMake 3.15 and up
#IF (POLICY CMP0091)
#    CMAKE_POLICY (SET CMP0091 NEW)
#ENDIF ()

project(PyPoc)

SET(CMAKE_CXX_STANDARD 11)
#SET(CMAKE_CXX_STANDARD_REQUIRED TRUE)
#SET(CMAKE_CXX_EXTENSIONS ON)

# On some Ubuntu versions and derivatives, a bug exists whereby enabling
# PIE compilation (Position Independent Executables) results in the
# `file` utility incorrectly recognising the compiled vegastrike-engine binary
# as a shared library instead of a position independent shared executable
# object.
#
# The effect of the bug is that vegastrike-engine can still be started from the
# command line but that it will not be recognised as an executable by GUI
# file managers such as Nautilus and Dolphin.
#
# To avoid this scenario, turn off this flag by default and let packagers
# on other distributions turn this on if their OS is able to correctly deal
# with Position Independent Executables.

## For more info, see:
## - https://bugs.launchpad.net/ubuntu/+source/file/+bug/1747711
## - https://github.com/vegastrike/Vega-Strike-Engine-Source/issues/94
##
#OPTION(ENABLE_PIE "Enable Position Independent Executables/Shared Libraries (NOT RECOMMENDED on Ubuntu/Mint)" OFF)
#IF (CMAKE_VERSION VERSION_LESS "3.14")
#    MESSAGE("-- CMake version older than 3.14; falling back to old PIE logic")
#    IF (ENABLE_PIE)
#        MESSAGE("!! Enabling Position Independent Executables/Shared Libraries (NOT RECOMMENDED on Ubuntu/Mint) !!")
#        ADD_COMPILE_OPTIONS("-fpie")
#        LINK_LIBRARIES("-pie")
#    ELSE (ENABLE_PIE)
#        MESSAGE("++ Disabling Position Independent Executables/Shared Libraries (Recommended on Ubuntu/Mint)")
#        IF (NOT MSVC)
#            ADD_COMPILE_OPTIONS("-fno-pie")
#            LINK_LIBRARIES("-no-pie")
#        ENDIF (NOT MSVC)
#    ENDIF (ENABLE_PIE)
#ELSE (CMAKE_VERSION VERSION_LESS "3.14")
#    MESSAGE("-- CMake version 3.14 or later; using preferred PIE logic")
#    UNSET(CMAKE_POSITION_INDEPENDENT_CODE)
#    IF (ENABLE_PIE)
#        MESSAGE("!! Enabling Position Independent Executables/Shared Libraries (NOT RECOMMENDED on Ubuntu/Mint) !!")
#        SET(CMAKE_POSITION_INDEPENDENT_CODE ON)
#    ELSE (ENABLE_PIE)
#        MESSAGE("++ Disabling Position Independent Executables/Shared Libraries (Recommended on Ubuntu/Mint)")
#        SET(CMAKE_POSITION_INDEPENDENT_CODE OFF)
#    ENDIF (ENABLE_PIE)
#ENDIF (CMAKE_VERSION VERSION_LESS "3.14")

add_compile_definitions("NV_CUBE_MAP" "BOOST_PYTHON_NO_PY_SIGNATURES" "BOOST_ALL_DYN_LINK" "$<$<CONFIG:Debug>:BOOST_DEBUG_PYTHON>" "$<$<CXX_COMPILER_ID:MSVC>:__x86_64__>" "$<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>" "$<$<CXX_COMPILER_ID:MSVC>:_CRT_NONSTDC_NO_DEPRECATE>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/wd4244>" "$<$<CXX_COMPILER_ID:MSVC>:/wd4267>" "$<$<CXX_COMPILER_ID:MSVC>:/wd4305>" "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Debug>>:/Z7>")
add_link_options("$<$<CXX_COMPILER_ID:MSVC>:/DEBUG>")
add_compile_options("$<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:-pipe>" "$<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:-Wall>" "$<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:-fvisibility=hidden>" "$<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>,$<OR:$<CONFIG:Debug>,$<CONFIG:Profiler>,$<CONFIG:RelWithDebInfo>>>:-Og>" "$<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>,$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>>:-g3>" "$<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>,$<CONFIG:Profiler>>:-pg>" "$<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>,$<CONFIG:Profiler>>:-g2>" "$<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>,$<CONFIG:Release>>:-O3>")

# Let cmake find our in-tree modules
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${Vega_Strike_SOURCE_DIR})

FIND_PACKAGE(Python3 REQUIRED COMPONENTS Development)

SET(BOOST_PYTHON_COMPONENT "python${Python3_VERSION_MAJOR}${Python3_VERSION_MINOR}")
FIND_PACKAGE(Boost REQUIRED COMPONENTS ${BOOST_PYTHON_COMPONENT} )   #log log_setup date_time system filesystem thread chrono atomic REQUIRED
SET(BOOST_PYTHON_NO_PY_SIGNATURES 1)

add_link_options("$<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:-Bsymbolic>")

add_executable(PyPoc main.cpp)

target_link_libraries(PyPoc PRIVATE Boost::boost)
