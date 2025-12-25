// The MIT License (MIT)
//
// Copyright © 2022-2025 Stephen G. Tuggy
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the “Software”), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#define PY_SSIZE_T_CLEAN
#include <boost/python.hpp>

#include "PyPocLibrary.h"

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
#include "opentelemetry/sdk/logs/logger_provider.h"
#include "opentelemetry/sdk/logs/logger_provider_factory.h"
#include "opentelemetry/logs/provider.h"
#include "opentelemetry/sdk/logs/provider.h"
#include "opentelemetry/sdk/logs/simple_log_record_processor_factory.h"
#include "opentelemetry/sdk/trace/provider.h"
#include "opentelemetry/trace/provider.h"
#include "opentelemetry/sdk/trace/simple_processor_factory.h"
#include "opentelemetry/sdk/trace/tracer_provider.h"
#include "opentelemetry/sdk/trace/tracer_provider_factory.h"
#include "opentelemetry/trace/tracer_provider.h"

namespace nostd = opentelemetry::nostd;

namespace
{
opentelemetry::exporter::otlp::OtlpFileExporterOptions opts;
opentelemetry::exporter::otlp::OtlpFileLogRecordExporterOptions log_opts;

std::shared_ptr<opentelemetry::sdk::trace::TracerProvider> tracer_provider;
std::shared_ptr<opentelemetry::sdk::logs::LoggerProvider> logger_provider;

void InitTracer()
{
  // Create OTLP exporter instance
  auto exporter   = opentelemetry::exporter::otlp::OtlpFileExporterFactory::Create(opts);
  auto processor  = opentelemetry::sdk::trace::SimpleSpanProcessorFactory::Create(std::move(exporter));
  tracer_provider = opentelemetry::sdk::trace::TracerProviderFactory::Create(std::move(processor));

  // Set the global trace provider
  std::shared_ptr<opentelemetry::trace::TracerProvider> api_provider = tracer_provider;
  opentelemetry::sdk::trace::Provider::SetTracerProvider(api_provider);
}

void CleanupTracer()
{
  // We call ForceFlush to prevent to cancel running exportings, It's optional.
  if (tracer_provider)
  {
    tracer_provider->ForceFlush();
  }

  tracer_provider = nullptr;
  std::shared_ptr<opentelemetry::trace::TracerProvider> none;
  opentelemetry::sdk::trace::Provider::SetTracerProvider(none);
}

void InitLogger()
{
  // Create OTLP exporter instance
  auto exporter   = opentelemetry::exporter::otlp::OtlpFileLogRecordExporterFactory::Create(log_opts);
  auto processor  = opentelemetry::sdk::logs::SimpleLogRecordProcessorFactory::Create(std::move(exporter));
  logger_provider = opentelemetry::sdk::logs::LoggerProviderFactory::Create(std::move(processor));

  std::shared_ptr<opentelemetry::logs::LoggerProvider> api_provider = logger_provider;
  opentelemetry::sdk::logs::Provider::SetLoggerProvider(api_provider);
}

void CleanupLogger()
{
  // We call ForceFlush to prevent to cancel running exportings, It's optional.
  if (logger_provider)
  {
    logger_provider->ForceFlush();
  }

  logger_provider = nullptr;
  nostd::shared_ptr<opentelemetry::logs::LoggerProvider> none;
  opentelemetry::logs::Provider::SetLoggerProvider(none);
}
}  // namespace

#endif

int main(int argc, char *argv[]) {
#if defined(USE_OPEN_TELEMETRY)
    if (argc > 1)
    {
        opentelemetry::exporter::otlp::OtlpFileClientFileSystemOptions fs_backend;
        fs_backend.file_pattern = argv[1];
        fs_backend.alias_pattern = "PyPoc_trace_latest.jsonl";
        opts.backend_options    = fs_backend;
        if (argc > 2)
        {
            opentelemetry::exporter::otlp::OtlpFileClientFileSystemOptions logs_fs_backend;
            logs_fs_backend.file_pattern = argv[2];
            logs_fs_backend.alias_pattern = "PyPoc_logs_latest.jsonl";
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
        log_opts.backend_options = std::ref(std::cout);
    }
    InitLogger();
    InitTracer();
#endif

    Py_Initialize();

    try {
        boost::python::object main_module = boost::python::import("__main__");
        boost::python::object main_namespace = main_module.attr("__dict__");
        boost::python::object ignored = boost::python::exec("print(\"Hello world from Python!\")", main_namespace);
    } catch (const boost::python::error_already_set&) {
        PyErr_Print();
        return 1;
    }

    auto library_instance = std::make_shared<py_poc::PyPocLibrary>();
    library_instance->foo();

#if defined(USE_OPEN_TELEMETRY)
    CleanupTracer();
    CleanupLogger();
#endif

    return 0;
}

//PyObject* MyPyFunc() {
//
//}
