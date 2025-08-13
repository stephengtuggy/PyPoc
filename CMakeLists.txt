cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

IF (POLICY CMP0087)
    cmake_policy(SET CMP0087 NEW)
ENDIF ()

IF (POLICY CMP0167)
    CMAKE_POLICY (SET CMP0167 NEW)
ENDIF (POLICY CMP0167)

project(PyPoc
        LANGUAGES CXX
        VERSION 0.2.1)

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_STANDARD_REQUIRED TRUE)
SET(CMAKE_CXX_EXTENSIONS ON)

SET(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
SET(CMAKE_INSTALL_UCRT_LIBRARIES TRUE)
INCLUDE(InstallRequiredSystemLibraries)

OPTION(ENABLE_PIE "Enable Position Independent Executables" ON)

include(CheckPIESupported)
check_pie_supported()
UNSET(CMAKE_POSITION_INDEPENDENT_CODE)
IF (ENABLE_PIE)
    SET(CMAKE_POSITION_INDEPENDENT_CODE ON)
ELSE (ENABLE_PIE)
    SET(CMAKE_POSITION_INDEPENDENT_CODE OFF)
ENDIF (ENABLE_PIE)

OPTION(USE_OPEN_TELEMETRY "Include and use opentelemetry-cpp" OFF)
IF (USE_OPEN_TELEMETRY)
    SET(CMAKE_MAP_IMPORTED_CONFIG_RELWITHDEBINFO "Debug")
    FIND_PACKAGE(Protobuf CONFIG)

    # Use find_package to include OpenTelemetry C++
    FIND_PACKAGE(opentelemetry-cpp CONFIG COMPONENTS api sdk exporters_otlp_file)
    #check whether opentelemetry libraries found
    IF (opentelemetry-cpp_FOUND)
        MESSAGE(STATUS "Found opentelemetry libraries; preparing to use them")
    ELSE ()
        MESSAGE(WARNING "Did not find opentelemetry libraries; building without")
        SET(USE_OPEN_TELEMETRY OFF)
    ENDIF ()
ELSE ()
    MESSAGE(STATUS "USE_OPEN_TELEMETRY not set; building without")
ENDIF ()

FIND_PACKAGE(Python3 REQUIRED COMPONENTS Development Interpreter)
SET(BOOST_PYTHON_COMPONENT "python${Python3_VERSION_MAJOR}${Python3_VERSION_MINOR}")
FIND_PACKAGE(Boost REQUIRED COMPONENTS ${BOOST_PYTHON_COMPONENT} )
include_directories(${Python3_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

add_executable(PyPoc main.cpp
        PyPocLibrary.cpp)
TARGET_COMPILE_DEFINITIONS(PyPoc PUBLIC "BOOST_ALL_DYN_LINK" "$<$<CONFIG:Debug>:BOOST_DEBUG_PYTHON>")
IF (WIN32)
    TARGET_COMPILE_DEFINITIONS(PyPoc PUBLIC BOOST_USE_WINAPI_VERSION=0x0A00)
    TARGET_COMPILE_DEFINITIONS(PyPoc PUBLIC _WIN32_WINNT=0x0A00)
    TARGET_COMPILE_DEFINITIONS(PyPoc PUBLIC WINVER=0x0A00)
    TARGET_COMPILE_DEFINITIONS(PyPoc PUBLIC "$<$<CONFIG:Debug>:Py_DEBUG>")
ENDIF()
IF (USE_OPEN_TELEMETRY)
    target_compile_definitions(PyPoc PRIVATE USE_OPEN_TELEMETRY=1)
    if(DEFINED OPENTELEMETRY_BUILD_DLL)
        target_compile_definitions(PyPoc
                PRIVATE OPENTELEMETRY_BUILD_IMPORT_DLL
        )
        target_link_libraries(PyPoc
                PRIVATE opentelemetry-cpp::opentelemetry_cpp)
    else()
        target_link_libraries(PyPoc
                PRIVATE opentelemetry-cpp::api
                opentelemetry-cpp::common
                opentelemetry-cpp::logs
                opentelemetry-cpp::metrics
                opentelemetry-cpp::resources
                opentelemetry-cpp::sdk
                opentelemetry-cpp::trace
                opentelemetry-cpp::otlp_file_exporter
                opentelemetry-cpp::otlp_file_metric_exporter
                opentelemetry-cpp::otlp_file_log_record_exporter
        )
    endif()
ENDIF ()

target_link_libraries(PyPoc PRIVATE ${Python3_LIBRARIES} ${Boost_LIBRARIES})
