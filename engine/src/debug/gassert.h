#include "debug/log/log.h"

#define GASSERT(condition, msg) if(!(condition)) { LOG_ERROR(msg); abort();}