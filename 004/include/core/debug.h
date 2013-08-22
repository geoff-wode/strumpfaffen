#if ! defined(__DEBUG_H__)
#define __DEBUG_H__

#include <stdio.h>
#include <stdarg.h>

extern FILE* errorFile;

#define LOG(msg, ...) \
	do \
	{ \
		if (NULL == errorFile) { errorFile = fopen("errors.txt", "wb"); } \
		fprintf(errorFile, msg, __VA_ARGS__); \
	} while (0)

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

#endif // __DEBUG_H__

