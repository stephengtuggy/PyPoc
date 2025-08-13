#define PY_SSIZE_T_CLEAN
#include <boost/python.hpp>

#if defined(USE_OPEN_TELEMETRY)
#include <functional>
#include <iostream>
#include <string>
#include <utility>

#include "opentelemetry/exporters/otlp/otlp_file_client_options.h"
#include "opentelemetry/exporters/otlp/otlp_file_exporter_factory.h"
#include "opentelemetry/exporters/otlp/otlp_file_exporter_options.h"
#include "opentelemetry/exporters/otlp/otlp_file_log_record_exporter_factory.h"
#include "opentelemetry/exporters/otlp/otlp_file_log_record_exporter_options.h"
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

namespace trace     = opentelemetry::trace;
namespace trace_sdk = opentelemetry::sdk::trace;
namespace otlp      = opentelemetry::exporter::otlp;

namespace
{
    opentelemetry::exporter::otlp::OtlpFileExporterOptions opts;

    std::shared_ptr<opentelemetry::sdk::trace::TracerProvider> provider;

    void InitTracer()
    {
        // Create OTLP exporter instance
        auto exporter  = otlp::OtlpFileExporterFactory::Create(opts);
        auto processor = trace_sdk::SimpleSpanProcessorFactory::Create(std::move(exporter));
        provider       = trace_sdk::TracerProviderFactory::Create(std::move(processor));

        // Set the global trace provider
        std::shared_ptr<opentelemetry::trace::TracerProvider> api_provider = provider;
        trace_sdk::Provider::SetTracerProvider(api_provider);
    }

    void CleanupTracer()
    {
        // We call ForceFlush to prevent to cancel running exportings, It's optional.
        if (provider)
        {
            provider->ForceFlush();
        }

        provider.reset();
        std::shared_ptr<opentelemetry::trace::TracerProvider> none;
        trace_sdk::Provider::SetTracerProvider(none);
    }
}  // namespace

int main(int argc, char *argv[]) {
    if (argc > 1)
    {
        opentelemetry::exporter::otlp::OtlpFileClientFileSystemOptions fs_backend;
        fs_backend.file_pattern = argv[1];
        opts.backend_options    = fs_backend;
    }
    // Removing this line will leave the default noop TracerProvider in place.
    InitTracer();

    Py_Initialize();

    try {
        boost::python::object main_module = boost::python::import("__main__");
        boost::python::object main_namespace = main_module.attr("__dict__");
        boost::python::object ignored = boost::python::exec("print(\"Hello world from Python!\")", main_namespace);
    } catch (const boost::python::error_already_set&) {
        PyErr_Print();
        return 1;
    }

    CleanupTracer();
    return 0;
}

//PyObject* MyPyFunc() {
//
//}
