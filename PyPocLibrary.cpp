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

namespace nostd = opentelemetry::nostd;

namespace
{
    nostd::shared_ptr<opentelemetry::trace::Tracer> get_tracer()
    {
        const nostd::shared_ptr<opentelemetry::trace::TracerProvider> provider = opentelemetry::trace::Provider::GetTracerProvider();
        return provider->GetTracer("py_poc_library");
    }

    nostd::shared_ptr<opentelemetry::logs::Logger> get_logger()
    {
        const nostd::shared_ptr<opentelemetry::logs::LoggerProvider> provider = opentelemetry::logs::Provider::GetLoggerProvider();
        return provider->GetLogger("py_poc_library_logger", "py_poc_library");
    }
}  // namespace

#endif

namespace py_poc {
    void PyPocLibrary::foo() {
#if defined(USE_OPEN_TELEMETRY)
        const nostd::shared_ptr<opentelemetry::trace::Span>  span        = get_tracer()->StartSpan("span 1");
        opentelemetry::trace::Scope                          scoped_span = opentelemetry::trace::Scope(get_tracer()->StartSpan("py_poc_library"));
        const opentelemetry::trace::SpanContext              ctx         = span->GetContext();
        const nostd::shared_ptr<opentelemetry::logs::Logger> logger      = get_logger();

        logger->Debug("Test Debug Message", ctx.trace_id(), ctx.span_id(), ctx.trace_flags());
        constexpr opentelemetry::logs::Severity severity = opentelemetry::logs::Severity::kDebug;
        logger->Log(severity, "Test Log Message");
#endif
    }
} // py_poc
