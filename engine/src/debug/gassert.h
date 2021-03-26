#include "debug/log/log.h"

#ifndef NDEBUG
   #define GASSERT(condition, msg) if(!(condition)) { LOG_ERROR("%s", msg); abort(); }
#else
   #define GASSERT(condition, msg) (condition)
#endif