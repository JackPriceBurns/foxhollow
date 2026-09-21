#ifndef BOREALIS_LOG_C_H
#define BOREALIS_LOG_C_H

/* printf-style logging for C and decompilation code. C++ should use borealis::Log. */

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    BOREALIS_LOG_TRACE,
    BOREALIS_LOG_DEBUG,
    BOREALIS_LOG_INFO,
    BOREALIS_LOG_WARNING,
    BOREALIS_LOG_ERROR,
    BOREALIS_LOG_FATAL,
} BorealisLogLevel;

#if defined(__GNUC__) || defined(__clang__)
#define BOREALIS_PRINTF_ATTR(fmt, args) __attribute__((format(printf, fmt, args)))
#else
#define BOREALIS_PRINTF_ATTR(fmt, args)
#endif

/* Log a preformatted message. */
void borealis_log_write(BorealisLogLevel level, const char* module, const char* message);

void borealis_log_vprintf(
    BorealisLogLevel level, const char* module, const char* format, va_list ap);

void borealis_log_printf(BorealisLogLevel level, const char* module, const char* format, ...)
    BOREALIS_PRINTF_ATTR(3, 4);

#ifdef __cplusplus
}
#endif

#endif
