#pragma once

typedef enum LogLevels
{
   LL_MESSAGE = 0,
   LL_ERROR,
   LL_WARNING
} LogLevels;
      
void PrintLog(const LogLevels level, const char* filepath, const int line, const const char* format, ...);
void PrintLogWD(const LogLevels level, const char* format, ...);