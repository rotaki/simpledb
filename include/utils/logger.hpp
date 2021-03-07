#pragma once

#include <ctime>
#include <string>

namespace simpledb {
#define LOG_LOG_TIME_FORMAT "%Y-%m-%d %H:%M:%S"
#define LOG_OUTPUT_STREAM stderr

// Log levels
#define LOG_LEVEL_OFF 0
#define LOG_LEVEL_ERROR 1
#define LOG_LEVEL_WARN 2
#define LOG_LEVEL_INFO 3
#define LOG_LEVEL_DEBUG 4
#define LOG_LEVEL_TRACE 5
#define LOG_LEVEL_ALL 6

// #ifndef LOG_LEVEL
// //#pragma message("Warning: LOG_LEVEL compile options was not explicitly
// given.") #ifndef NDEBUG #pragma message("LOG_LEVEL_DEBUG is used instead as
// DEBUG options is on.") static constexpr int LOG_LEVEL = LOG_LEVEL_DEBUG;
// #else
// #pragma message("LOG_LEVEL_WARN is used instead as DEBUG options is on.")
// static constexpr int LOG_LEVEL = LOG_LEVEL_WARN;
// #endif /* NDEBUG */
// #pragma message("Give LOG_LEVEL compile option to overwrite the default
// level.") #endif /* LOG_LEVEL */

// https://blog.galowicz.de/2016/02/20/short_file_macro/
using cstr = const char *;
static constexpr cstr PastLastSlash(cstr a, cstr b) {
  return *a == '\0'  ? b
         : *b == '/' ? PastLastSlash(a + 1, a + 1)
                     : PastLastSlash(a + 1, b);
}
static constexpr cstr PastLastSlash(cstr a) { return PastLastSlash(a, a); }
#define __SHORT_FILE__                                                         \
  ({                                                                           \
    constexpr cstr sf__{PastLastSlash(__FILE__)};                              \
    sf__;                                                                      \
  })

void OutputLogHeader(const char *file_name, int line, const char *func,
                     int level);

#ifdef LOG_ERROR_ENABLED
#undef LOG_ERROR_ENABLED
#endif
#if LOG_LEVEL >= LOG_LEVEL_ERROR
#define LOG_ERROR_ENABLED
#define LOG_ERROR(...)                                                         \
  OutputLogHeader(__SHORT_FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_ERROR);    \
  ::fprintf(LOG_OUTPUT_STREAM, __VA_ARGS__);                                   \
  ::fprintf(LOG_OUTPUT_STREAM, "\n");                                          \
  ::fflush(LOG_OUTPUT_STREAM)
#else

#define LOG_ERROR(...) ((void)0)
#endif

#ifdef LOG_WARN_ENABLED
#undef LOG_WARN_ENABLED
#endif
#if LOG_LEVEL >= LOG_LEVEL_WARN
#define LOG_WARN_ENABLED
#define LOG_WARN(...)                                                          \
  OutputLogHeader(__SHORT_FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_WARN);     \
  ::fprintf(LOG_OUTPUT_STREAM, __VA_ARGS__);                                   \
  ::fprintf(LOG_OUTPUT_STREAM, "\n");                                          \
  ::fflush(LOG_OUTPUT_STREAM)
#else
#define LOG_WARN(...) ((void)0)
#endif

#ifdef LOG_INFO_ENABLED
#undef LOG_INFO_ENBALED
#endif
#if LOG_LEVEL >= LOG_LEVEL_INFO
#define LOG_INFO_ENABLED
#define LOG_INFO(...)                                                          \
  OutputLogHeader(__SHORT_FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_INFO);     \
  ::fprintf(LOG_OUTPUT_STREAM, __VA_ARGS__);                                   \
  ::fprintf(LOG_OUTPUT_STREAM, "\n");                                          \
  ::fflush(LOG_OUTPUT_STREAM)
#else
#define LOG_INFO(...) ((void)0)
#endif

#ifdef LOG_DEBUG_ENABLED
#undef LOG_DEBUG_ENBALED
#endif
#if LOG_LEVEL >= LOG_LEVEL_DEBUG
#define LOG_DEBUG_ENABLED
#define LOG_DEBUG(...)                                                         \
  OutputLogHeader(__SHORT_FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_DEBUG);    \
  ::fprintf(LOG_OUTPUT_STREAM, __VA_ARGS__);                                   \
  ::fprintf(LOG_OUTPUT_STREAM, "\n");                                          \
  ::fflush(LOG_OUTPUT_STREAM)
#else
#define LOG_DEBUG(...) ((void)0)
#endif

#ifdef LOG_TRACE_ENABLED
#undef LOG_TRACE_ENBALED
#endif
#if LOG_LEVEL >= LOG_LEVEL_TRACE
#define LOG_TRACE_ENABLED
#define LOG_TRACE(...)                                                         \
  OutputLogHeader(__SHORT_FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_TRACE);    \
  ::fprintf(LOG_OUTPUT_STREAM, __VA_ARGS__);                                   \
  ::fprintf(LOG_OUTPUT_STREAM, "\n");                                          \
  ::fflush(LOG_OUTPUT_STREAM)
#else
#define LOG_TRACE(...) ((void)0)
#endif

inline void OutputLogHeader(const char *file_name, int line, const char *func,
                            int level) {
  time_t t = ::time(nullptr);
  tm *curTime = localtime(&t);
  char time_str[32];
  ::strftime(time_str, 32, LOG_LOG_TIME_FORMAT, curTime);
  const char *type;
  switch (level) {
  case LOG_LEVEL_ERROR:
    type = "ERROR";
    break;
  case LOG_LEVEL_WARN:
    type = "WARN";
    break;
  case LOG_LEVEL_INFO:
    type = "INFO";
    break;
  case LOG_LEVEL_DEBUG:
    type = "DEBUG";
    break;
  case LOG_LEVEL_TRACE:
    type = "TRACE";
    break;
  default:
    type = "UNKWN";
  }
  ::fprintf(LOG_OUTPUT_STREAM, "%s [%s:%d:%s] %s - ", time_str, file_name, line,
            func, type);
}
} // namespace simpledb
