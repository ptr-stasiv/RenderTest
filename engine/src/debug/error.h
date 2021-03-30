#pragma once
#include <string_view>

#ifdef WIN64
   #include "platforms/win64/win64-dev.h"
#endif

namespace err
{
   inline void CreatePlatformMB(const std::string_view& msg);

   inline void CreateMBAndTerminate(const std::string_view& msg)
   {
      CreatePlatformMB(msg);
      abort();
   }

   inline void CreatePlatformMB(const std::string_view& msg)
   {
#ifdef WIN64
      MessageBoxA(NULL, msg.data(), NULL, MB_OK);
#endif
   }
}