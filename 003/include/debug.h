#if ! defined(__DEBUG_H__)
#define __DEBUG_H__

#include <stdio.h>
#include <stdarg.h>

#if defined(ENABLE_DEBUG)

#define LOG(msg, ...) fprintf(stderr, msg, __VA_ARGS__)

#define ASSERT(pred) ASSERTM(pred, "")

#define ASSERTM(pred, msg, ...) \
  do \
  { \
    if (!(pred)) \
    { \
      LOG("%s:%d:%s - %s", #pred, msg, __VA_ARGS__); \
      assert(0); \
    } \
  } while (0)

#else // ! ENABLE_DEBUG

#define LOG(msg, ...)	do { } while (0)

#define ASSERT(pred)	do { } while (0)

#define ASSERTM(pred, msg, ...) do { } while (0)

#endif // ENABLE_DEBUG

#endif // __DEBUG_H__
