#include "log.h"

#include <stdio.h>
#include <stdarg.h>
#include <Windows.h>

static const char* g_FontColorModes[] =
{
   "\033[0m",  //Default
   "\033[31m", //Red
   "\033[33m"  //Yellow
};

void PrintLog(const LogLevels level, const char* filepath, const int line, const const char* format, ...)
{
   SYSTEMTIME st;
   GetLocalTime(&st);

   int colorMode;
   char message[8];

   switch (level)
   {
   case LL_MESSAGE:
      {
         colorMode = 0;
         strcpy(message, "Message");
      }break;
   case LL_ERROR:
      {
         colorMode = 1;
         strcpy(message, "Error");
      }break;
   case LL_WARNING:
      {
         colorMode = 2;
         strcpy(message, "Warning");
      }break;
   default:
      break;
   }

   printf(g_FontColorModes[colorMode]);

   printf("- %d:%d:%d %s %s:%d ", st.wHour, st.wMinute, st.wSecond, message, filepath, line);
  
   va_list args;
   va_start(args, format);
   vprintf(format, args);
   va_end(args);

   printf(g_FontColorModes[0]);
   printf("\n");
}