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
namespace nostd     = opentelemetry::nostd;
namespace otlp      = opentelemetry::exporter::otlp;
namespace logs_sdk  = opentelemetry::sdk::logs;
namespace logs      = opentelemetry::logs;
namespace trace_sdk = opentelemetry::sdk::trace;

namespace
{
    opentelemetry::exporter::otlp::OtlpFileExporterOptions opts;
    opentelemetry::exporter::otlp::OtlpFileLogRecordExporterOptions log_opts;

    std::shared_ptr<opentelemetry::sdk::trace::TracerProvider> tracer_provider;
    std::shared_ptr<opentelemetry::sdk::logs::LoggerProvider> logger_provider;

    void InitTracer()
    {
        // Create OTLP exporter instance
        auto exporter   = otlp::OtlpFileExporterFactory::Create(opts);
        auto processor  = trace_sdk::SimpleSpanProcessorFactory::Create(std::move(exporter));
        tracer_provider = trace_sdk::TracerProviderFactory::Create(std::move(processor));

        // Set the global trace provider
        std::shared_ptr<opentelemetry::trace::TracerProvider> api_provider = tracer_provider;
        trace_sdk::Provider::SetTracerProvider(api_provider);
    }

    void CleanupTracer()
    {
        // We call ForceFlush to prevent to cancel running exportings, It's optional.
        if (tracer_provider)
        {
            tracer_provider->ForceFlush();
        }

        tracer_provider.reset();
        std::shared_ptr<opentelemetry::trace::TracerProvider> none;
        trace_sdk::Provider::SetTracerProvider(none);
    }

    void InitLogger()
    {
        // Create OTLP exporter instance
        auto exporter   = otlp::OtlpFileLogRecordExporterFactory::Create(log_opts);
        auto processor  = logs_sdk::SimpleLogRecordProcessorFactory::Create(std::move(exporter));
        logger_provider = logs_sdk::LoggerProviderFactory::Create(std::move(processor));

        std::shared_ptr<opentelemetry::logs::LoggerProvider> api_provider = logger_provider;
        logs_sdk::Provider::SetLoggerProvider(api_provider);
    }

    void CleanupLogger()
    {
        // We call ForceFlush to prevent to cancel running exportings, It's optional.
        if (logger_provider)
        {
            logger_provider->ForceFlush();
        }

        logger_provider.reset();
        nostd::shared_ptr<logs::LoggerProvider> none;
        logs_sdk::Provider::SetLoggerProvider(none);
    }
}  // namespace

int main(int argc, char *argv[]) {
    if (argc > 1)
    {
        opentelemetry::exporter::otlp::OtlpFileClientFileSystemOptions fs_backend;
        fs_backend.file_pattern = argv[1];
        opts.backend_options    = fs_backend;
        if (argc > 2)
        {
            opentelemetry::exporter::otlp::OtlpFileClientFileSystemOptions logs_fs_backend;
            logs_fs_backend.file_pattern = argv[2];
            log_opts.backend_options     = logs_fs_backend;
        }
        else
        {
            log_opts.backend_options = std::ref(std::cout);
        }
    }
    else
    {
        opts.backend_options = std::ref(std::cout);
    }
    InitLogger();
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
    CleanupLogger();
    return 0;
}

//PyObject* MyPyFunc() {
//
//}
