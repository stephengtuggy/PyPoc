# The MIT License (MIT)
#
# Copyright © 2022-2026 Stephen G. Tuggy
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the “Software”), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

CMAKE_MINIMUM_REQUIRED(VERSION 3.25...4.3 FATAL_ERROR)

IF (POLICY CMP0087)
    CMAKE_POLICY(SET CMP0087 NEW)
ENDIF ()

IF (POLICY CMP0091)
    CMAKE_POLICY(SET CMP0091 NEW)
ENDIF ()

IF (POLICY CMP0102)
    CMAKE_POLICY(SET CMP0102 OLD)
ENDIF ()

IF (POLICY CMP0126)
    CMAKE_POLICY(SET CMP0126 NEW)
ENDIF ()

IF (POLICY CMP0167)
    CMAKE_POLICY(SET CMP0167 NEW)
ENDIF ()

IF (POLICY CMP0207)
    CMAKE_POLICY(SET CMP0207 NEW)
ENDIF ()

SET(CMAKE_BUILD_PARALLEL_LEVEL 3)

IF (WIN32)
    SET(VCPKG_MANIFEST_MODE ON)
    SET(VCPKG_MANIFEST_DIR "${CMAKE_SOURCE_DIR}")
    SET(VCPKG_MANIFEST_INSTALL ON)
ENDIF (WIN32)

PROJECT(PyPoc
        LANGUAGES CXX
        VERSION 0.4.2)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED TRUE)
SET(CMAKE_CXX_EXTENSIONS ON)

SET(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
#SET(CMAKE_INSTALL_UCRT_LIBRARIES TRUE)
INCLUDE(InstallRequiredSystemLibraries)

OPTION(ENABLE_PIE "Enable Position Independent Executables" ON)

INCLUDE(CheckPIESupported)
CHECK_PIE_SUPPORTED()
UNSET(CMAKE_POSITION_INDEPENDENT_CODE)
IF (ENABLE_PIE)
    SET(CMAKE_POSITION_INDEPENDENT_CODE ON)
ELSE (ENABLE_PIE)
    SET(CMAKE_POSITION_INDEPENDENT_CODE OFF)
ENDIF (ENABLE_PIE)

ADD_COMPILE_OPTIONS(
        "$<$<CXX_COMPILER_ID:MSVC>:/MP>"
        "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Debug>>:/Z7>"
)
ADD_LINK_OPTIONS(
        "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Debug>>:/DEBUG>"
        "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:RelWithDebInfo>>:/DEBUG>"
)

INCLUDE(CMakeDependentOption)
OPTION(FETCH_OPEN_TELEMETRY "Whether to download opentelemetry-cpp using FetchContent_MakeAvailable" OFF)
OPTION(USE_OPEN_TELEMETRY "Whether to use opentelemetry-cpp, regardless of how it is installed" OFF)
cmake_dependent_option(USE_LOCALLY_INSTALLED_PROTOBUF "Whether to use locally-installed copy of Protobuf" OFF "USE_OPEN_TELEMETRY" OFF)
cmake_dependent_option(FETCH_PROTOBUF "Whether to download Protobuf using FetchContent_MakeAvailable" ON "USE_OPEN_TELEMETRY;NOT USE_LOCALLY_INSTALLED_PROTOBUF" OFF)
cmake_dependent_option(USE_LOCALLY_INSTALLED_ZLIB "Whether to use locally-installed copy of ZLIB" OFF "USE_OPEN_TELEMETRY" OFF)
cmake_dependent_option(FETCH_ZLIB "Whether to download ZLIB using FetchContent_MakeAvailable" ON "USE_OPEN_TELEMETRY;NOT USE_LOCALLY_INSTALLED_ZLIB" OFF)
cmake_dependent_option(USE_LOCALLY_INSTALLED_ABSEIL "Whether to use locally-installed copy of Abseil" OFF "USE_OPEN_TELEMETRY" OFF)
cmake_dependent_option(FETCH_ABSEIL "Whether to download Abseil using FetchContent_MakeAvailable" ON "USE_OPEN_TELEMETRY;NOT USE_LOCALLY_INSTALLED_ABSEIL" OFF)

INCLUDE(FetchContent)

IF (USE_OPEN_TELEMETRY)
    SET(ABSL_ENABLE_INSTALL ON)
    SET(ABSL_MSVC_STATIC_RUNTIME ON)
    SET(ABSL_USE_EXTERNAL_GOOGLETEST ON)
    SET(ABSL_PROPAGATE_CXX_STD ON)
    IF (USE_LOCALLY_INSTALLED_ABSEIL)
        FIND_PACKAGE(absl CONFIG REQUIRED)
    ELSEIF (FETCH_ABSEIL)
        FETCHCONTENT_DECLARE(
                absl
                GIT_REPOSITORY      https://github.com/abseil/abseil-cpp.git
                GIT_TAG             76bb24329e8bf5f39704eb10d21b9a80befa7c81                # 20250512.1 / 20250512.2
        )
        MESSAGE(STATUS "Downloading absl")
        FETCHCONTENT_MAKEAVAILABLE(absl)
        MESSAGE(STATUS "Finished fetching absl")
    ENDIF ()

    IF (USE_LOCALLY_INSTALLED_ZLIB)
        FIND_PACKAGE(ZLIB REQUIRED)
    ELSEIF (FETCH_ZLIB)
        FETCHCONTENT_DECLARE(
                zlib
                GIT_REPOSITORY      https://github.com/madler/zlib.git
                GIT_TAG             da607da739fa6047df13e66a2af6b8bec7c2a498                # v1.3.2
        )
        MESSAGE(STATUS "Downloading zlib")
        FETCHCONTENT_MAKEAVAILABLE(zlib)
        MESSAGE(STATUS "Finished fetching zlib")
    ENDIF ()

    IF(TARGET zlib)
        SET_TARGET_PROPERTIES(zlib PROPERTIES POSITION_INDEPENDENT_CODE ON MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    ENDIF ()

    SET(protobuf_INSTALL ON CACHE BOOL "" FORCE)
    SET(protobuf_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    SET(protobuf_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    SET(protobuf_MSVC_STATIC_RUNTIME OFF)
    SET(protobuf_ABSL_PROVIDER "package")
    SET(protobuf_WITH_ZLIB ON)
    SET(protobuf_ZLIB_PROVIDER "package")
    IF (USE_LOCALLY_INSTALLED_PROTOBUF)
        FIND_PACKAGE(Protobuf CONFIG REQUIRED)
        SET(protobuf_PROVIDER "find_package")
    ELSEIF (FETCH_PROTOBUF)
        FETCHCONTENT_DECLARE(
                Protobuf
                GIT_REPOSITORY      https://github.com/protocolbuffers/protobuf.git
                GIT_TAG             35cd01f9fe9afbeea38cc7b979a3b6bfcde82c03                # v35.1
        )
        MESSAGE(STATUS "Downloading Protobuf")
        FETCHCONTENT_MAKEAVAILABLE(Protobuf)
        MESSAGE(STATUS "Finished fetching Protobuf")
        SET(protobuf_PROVIDER "fetch_repository")
    ENDIF ()

    IF(TARGET libprotobuf)
        SET_TARGET_PROPERTIES(libprotobuf PROPERTIES POSITION_INDEPENDENT_CODE ON CXX_CLANG_TIDY "" CXX_INCLUDE_WHAT_YOU_USE "" MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    ENDIF ()
ENDIF ()

IF (FETCH_OPEN_TELEMETRY)
    FETCHCONTENT_DECLARE(
            opentelemetry-cpp
            GIT_REPOSITORY      https://github.com/open-telemetry/opentelemetry-cpp.git
            GIT_TAG             e51997b191565bdac2d72ab62021f8580dc7d01c                # Latest on main as of 2026-10-02
            )
    SET(OTELCPP_WITH_OTLP_FILE ON CACHE BOOL "" FORCE)
    SET(OTELCPP_WITH_BENCHMARK OFF CACHE BOOL "")
    SET(OTELCPP_WITH_EXAMPLES OFF CACHE BOOL "")
    SET(OTELCPP_WITH_FUNC_TESTS OFF CACHE BOOL "")
    SET(OTELCPP_BUILD_TESTING OFF CACHE BOOL "")
    SET(OTELCPP_WITH_ABI_VERSION_1 OFF CACHE BOOL "" FORCE)
    SET(OTELCPP_WITH_ABI_VERSION_2 ON CACHE BOOL "" FORCE)
    SET(OTELCPP_CMAKE_POSITION_INDEPENDENT_CODE ON CACHE BOOL "" FORCE)
    SET(OTELCPP_BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
    SET(OTELCPP_INSTALL ON CACHE BOOL "" FORCE)
    IF (WIN32)
        SET(OTELCPP_BUILD_DLL ON CACHE BOOL "" FORCE)
        SET(OPENTELEMETRY_BUILD_IMPORT_DLL ON CACHE BOOL "" FORCE)
    ENDIF ()
    MESSAGE(STATUS "Downloading opentelemetry-cpp")
    FETCHCONTENT_MAKEAVAILABLE(opentelemetry-cpp)
    MESSAGE(STATUS "Finished fetching opentelemetry-cpp")
ELSEIF (USE_OPEN_TELEMETRY)
    #    SET(CMAKE_MAP_IMPORTED_CONFIG_RELWITHDEBINFO "Debug")

    # Use find_package to include OpenTelemetry C++
    FIND_PACKAGE(opentelemetry-cpp CONFIG COMPONENTS api sdk exporters_otlp_file)
    #check whether opentelemetry libraries found
    IF (opentelemetry-cpp_FOUND)
        MESSAGE(STATUS "Found opentelemetry libraries; preparing to use them")
        IF (WIN32)
            SET(OPENTELEMETRY_BUILD_IMPORT_DLL ON CACHE BOOL "" FORCE) # ??
        ENDIF ()
    ELSE ()
        MESSAGE(WARNING "Did not find opentelemetry libraries; building without")
        SET(USE_OPEN_TELEMETRY OFF)
    ENDIF ()
ELSE ()
    MESSAGE(STATUS "USE_OPEN_TELEMETRY not set; building without")
ENDIF ()

FIND_PACKAGE(Python3 REQUIRED COMPONENTS Development Interpreter)
SET(BOOST_PYTHON_COMPONENT "python${Python3_VERSION_MAJOR}${Python3_VERSION_MINOR}")
FIND_PACKAGE(Boost REQUIRED COMPONENTS ${BOOST_PYTHON_COMPONENT})

ADD_EXECUTABLE(PyPoc main.cpp
        PyPocLibrary.cpp)
TARGET_COMPILE_DEFINITIONS(PyPoc PUBLIC "BOOST_ALL_DYN_LINK" "$<$<CONFIG:Debug>:BOOST_DEBUG_PYTHON>")
IF (WIN32)
    TARGET_COMPILE_DEFINITIONS(PyPoc PUBLIC BOOST_USE_WINAPI_VERSION=0x0A00)
    TARGET_COMPILE_DEFINITIONS(PyPoc PUBLIC _WIN32_WINNT=0x0A00)
    TARGET_COMPILE_DEFINITIONS(PyPoc PUBLIC WINVER=0x0A00)
    TARGET_COMPILE_DEFINITIONS(PyPoc PUBLIC "$<$<CONFIG:Debug>:Py_DEBUG>")
ENDIF ()

TARGET_INCLUDE_DIRECTORIES(PyPoc SYSTEM PRIVATE ${Python3_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

IF (USE_OPEN_TELEMETRY)
    TARGET_COMPILE_DEFINITIONS(PyPoc PRIVATE USE_OPEN_TELEMETRY=1)
    TARGET_INCLUDE_DIRECTORIES(PyPoc SYSTEM PRIVATE ${OPENTELEMETRY_CPP_INCLUDE_DIRS})
    TARGET_LINK_LIBRARIES(PyPoc PRIVATE
            opentelemetry-cpp::api
            opentelemetry-cpp::common
            opentelemetry-cpp::logs
            opentelemetry-cpp::metrics
            opentelemetry-cpp::resources
            opentelemetry-cpp::sdk
            opentelemetry-cpp::trace
            opentelemetry-cpp::otlp_common
            opentelemetry-cpp::proto
            opentelemetry-cpp::otlp_recordable
            opentelemetry-cpp::otlp_file_exporter
            opentelemetry-cpp::otlp_file_metric_exporter
            opentelemetry-cpp::otlp_file_log_record_exporter
            protobuf::libprotobuf
            absl::base
    )
ENDIF ()

TARGET_LINK_LIBRARIES(PyPoc PRIVATE ${Python3_LIBRARIES} ${Boost_LIBRARIES})

SET_TARGET_PROPERTIES(
        PyPoc
        PROPERTIES
        INSTALL_RPATH
        "${CMAKE_INSTALL_RPATH};${CMAKE_INSTALL_PREFIX}/lib")

INSTALL(
        TARGETS PyPoc
        RUNTIME_DEPENDENCIES
        DIRECTORIES $ENV{PATH}
        PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-"
        POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
        DESTINATION bin)
