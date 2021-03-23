#pragma once
#include <Windows.h>
#include <string_view>

namespace utils
{
   struct ProcessInfo
   {
      HANDLE JobHandle;

      STARTUPINFO StartupInfo;
      PROCESS_INFORMATION ProcessInfo;
   };

   ProcessInfo CreateChildProcess(const std::string_view& execLocation, const std::string_view& workingDir);

   void DestroyProcess(const ProcessInfo& processInfo);
}