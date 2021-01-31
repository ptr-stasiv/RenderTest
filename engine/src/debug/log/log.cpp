#include "log.h"

#include <cstdio>
#include <cstdarg>
#include <Windows.h>

#include "utils/sync/spin-lock.h"

static const char* g_FontColorModes[] =
{
   "\033[0m",  //Default
   "\033[31m", //Red
   "\033[33m"  //Yellow
};

static utils::sync::SpinLock g_CmdSpinLock;

void PrintLog(const LogLevels level, const char* filepath, const int line, const const char* format, ...)
{
   SYSTEMTIME st;
   GetLocalTime(&st);

   char message[8];

   switch (level)
   {
   case LL_MESSAGE:
         strcpy(message, "Message"); break;
   case LL_ERROR:
         strcpy(message, "Error"); break;
   case LL_WARNING:
         strcpy(message, "Warning"); break;
   default:
      break;
   }

   g_CmdSpinLock.lock();

   printf("%s- %d:%d:%d %s %s:%d ", g_FontColorModes[level], st.wHour, st.wMinute, st.wSecond, message, filepath, line);
  
   va_list args;
   va_start(args, format);
   vprintf(format, args);
   va_end(args);

   printf("%s\n\n", g_FontColorModes[0]);

   g_CmdSpinLock.unlock();
}

void PrintLogWD(const LogLevels level, const const char* format, ...)
{
   g_CmdSpinLock.lock();

   printf("%s- ", g_FontColorModes[level]);

   va_list args;
   va_start(args, format);
   vprintf(format, args);
   va_end(args);

   printf("%s\n\n", g_FontColorModes[0]);

   g_CmdSpinLock.unlock();
}