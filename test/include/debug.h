#if ! defined(__DEBUG_H__)
#define __DEBUG_H__

#include <stdio.h>
#include <stdarg.h>

extern FILE* debugLogFile;

#define LOG(msg, ...) \
	do \
	{ \
		fprintf(debugLogFile, msg, __VA_ARGS__); \
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


// Given a GL data type, return a text equivalent.
const char* const GetAttribTypeName(unsigned int type);

#endif // __DEBUG_H__

