#define PY_SSIZE_T_CLEAN
//#include <Python.h>
#include <boost/python.hpp>
//#include <iostream>
//#include <stdlib.h>

#if defined(USE_OPEN_TELEMETRY)
#include <functional>
#include <iostream>
#include <string>
#include <utility>

// #include "opentelemetry/exporters/otlp/otlp_file_client_options.h"
// #include "opentelemetry/exporters/otlp/otlp_file_exporter_factory.h"
// #include "opentelemetry/exporters/otlp/otlp_file_exporter_options.h"
// #include "opentelemetry/exporters/otlp/otlp_file_log_record_exporter_factory.h"
// #include "opentelemetry/exporters/otlp/otlp_file_log_record_exporter_options.h"
#include "opentelemetry/logs/logger_provider.h"
#include "opentelemetry/nostd/shared_ptr.h"
#include "opentelemetry/sdk/logs/exporter.h"
#include "opentelemetry/sdk/logs/logger_provider.h"
#include "opentelemetry/sdk/logs/logger_provider_factory.h"
#include "opentelemetry/sdk/logs/processor.h"
#include "opentelemetry/sdk/logs/provider.h"
#include "opentelemetry/sdk/logs/simple_log_record_processor_factory.h"
#include "opentelemetry/sdk/trace/exporter.h"
#include "opentelemetry/sdk/trace/processor.h"
#include "opentelemetry/sdk/trace/provider.h"
#include "opentelemetry/sdk/trace/simple_processor_factory.h"
#include "opentelemetry/sdk/trace/tracer_provider.h"
#include "opentelemetry/sdk/trace/tracer_provider_factory.h"
#include "opentelemetry/trace/tracer_provider.h"
#endif

int main() {
//    setenv("PYTHONPATH", ".", 1);
//    std::string modpaths("\"\",");
//    modpaths += "r\".\"";
//    std::string changepath("import sys\nprint(sys.path)\nsys.path = [" + modpaths + "] + sys.path\n");

    Py_Initialize();

    try {
        boost::python::object main_module = boost::python::import("__main__");
        boost::python::object main_namespace = main_module.attr("__dict__");
        boost::python::object ignored = boost::python::exec("print(\"Hello world from Python!\")", main_namespace);
    } catch (const boost::python::error_already_set&) {
        PyErr_Print();
        return 1;
    }

    return 0;
}

//PyObject* MyPyFunc() {
//
//}
