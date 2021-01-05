#pragma once

typedef enum LogLevels
{
   LL_MESSAGE = 0,
   LL_ERROR,
   LL_WARNING
} LogLevels;
      
void PrintLog(const LogLevels level, const char* filepath, const int line, const const char* format, ...);
void PrintLogWD(const LogLevels level, const const char* format, ...);

#define LOG_MESSAGE(format, ...) PrintLog(LL_MESSAGE, __FILE__, __LINE__,format, __VA_ARGS__)
#define LOG_ERROR(format, ...)   PrintLog(LL_ERROR, __FILE__, __LINE__, format, __VA_ARGS__);
#define LOG_WARNING(format, ...) PrintLog(LL_WARNING, __FILE__, __LINE__, format, __VA_ARGS__)

#define WD_LOG_MESSAGE(format, ...) PrintLogWD(LL_MESSAGE, format, __VA_ARGS__)
#define WD_LOG_ERROR(format, ...)   PrintLogWD(LL_ERROR, format, __VA_ARGS__)
#define WD_LOG_WARNING(format, ...) PrintLogWD(LL_WARNING, format, __VA_ARGS__)