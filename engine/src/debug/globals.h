#pragma once
#include <cstdio>
#include <cstdarg>
#include <string>

#include "log/log.h"


//*LOG* macroses should only be used to print some information in console. 
//And if this being used to notify about problem this problem should not provoke other problems

//ASSERT can be used to test for ciritical errors which normally can only occur during development
//For ex. it can test the validity of shader uniform location

//PRINT_AND_TERMINATE showing the error and terminate program work as in debug as in release


#define LOG_MESSAGE(format, ...) PrintLog(LL_MESSAGE, __FILE__, __LINE__,format, __VA_ARGS__)
#define LOG_ERROR(format, ...)   PrintLog(LL_ERROR, __FILE__, __LINE__, format, __VA_ARGS__);
#define LOG_WARNING(format, ...) PrintLog(LL_WARNING, __FILE__, __LINE__, format, __VA_ARGS__)

#define WD_LOG_MESSAGE(format, ...) PrintLogWD(LL_MESSAGE, format, __VA_ARGS__)
#define WD_LOG_ERROR(format, ...)   PrintLogWD(LL_ERROR, format, __VA_ARGS__)
#define WD_LOG_WARNING(format, ...) PrintLogWD(LL_WARNING, format, __VA_ARGS__)


#ifdef WINDOWS 
   #define DEBUG_BREAK __debugbreak()
#else
   #define DEBUG_BREAK
#endif


#ifdef DEBUG
   #define ASSERT(condition, format, ...) if(!(condition)) \
                                          { \
                                            LOG_ERROR("Assert!"); \
                                            WD_LOG_ERROR("\t" format, __VA_ARGS__); \
                                            DEBUG_BREAK; \
                                          }
#else
   #define ASSERT(condition, format, ...)
#endif


namespace err
{
   void CreateHostMessageBox(const char* title, const char* message);

   inline std::string ResolveFormatString(const char* format, ...)
   {
      char buf[512];

      va_list args;
      va_start(args, format);
      vsprintf(buf, format, args);
      va_end(args);

      return buf;
   }
}

#ifdef DEBUG
   #define PRINT_AND_TERMINATE(format, ...) ASSERT(false, format, __VA_ARGS__)
#else
   #define PRINT_AND_TERMINATE(format, ...) { \
                                              err::CreateHostMessageBox("Critical error!", \
                                              err::ResolveFormatString(format, __VA_ARGS__).c_str());\
                                              abort(); \
                                            }
#endif
