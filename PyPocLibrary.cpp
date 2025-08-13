//
// Created by sgt on 8/12/2025.
//

#include "PyPocLibrary.h"

#if defined(USE_OPEN_TELEMETRY)
#include "opentelemetry/logs/logger.h"
#include "opentelemetry/logs/logger_provider.h"
#include "opentelemetry/logs/provider.h"
#include "opentelemetry/nostd/shared_ptr.h"
#include "opentelemetry/nostd/string_view.h"
#include "opentelemetry/trace/provider.h"
#include "opentelemetry/trace/scope.h"
#include "opentelemetry/trace/span.h"
#include "opentelemetry/trace/span_context.h"
#include "opentelemetry/trace/tracer.h"
#include "opentelemetry/trace/tracer_provider.h"

namespace logs  = opentelemetry::logs;
namespace trace = opentelemetry::trace;

namespace
{
    opentelemetry::nostd::shared_ptr<trace::Tracer> get_tracer()
    {
        auto provider = trace::Provider::GetTracerProvider();
        return provider->GetTracer("py_poc_library");
    }

    opentelemetry::nostd::shared_ptr<logs::Logger> get_logger()
    {
        auto provider = logs::Provider::GetLoggerProvider();
        return provider->GetLogger("py_poc_library_logger", "py_poc_library");
    }
}  // namespace

#endif

namespace py_poc {
    void PyPocLibrary::foo() {
#if defined(USE_OPEN_TELEMETRY)
        auto span        = get_tracer()->StartSpan("span 1");
        auto scoped_span = trace::Scope(get_tracer()->StartSpan("py_poc_library"));
        auto ctx         = span->GetContext();
        auto logger      = get_logger();

        logger->Debug("body", ctx.trace_id(), ctx.span_id(), ctx.trace_flags());
#endif
    }
} // py_poc
