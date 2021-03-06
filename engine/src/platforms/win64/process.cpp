#include "utils/process/process.h"

#include <Windows.h>

#include "debug/log/log.h"

namespace utils
{
   struct ProcessHandle::NativeInfo
   {
      STARTUPINFO StartupInfo;
      PROCESS_INFORMATION ProcessInfo;
   };

   ProcessHandle::ProcessHandle(const std::string_view& appName, const std::string_view& directory)
   {
      Native = std::make_unique<NativeInfo>();

      ZeroMemory(&Native->StartupInfo, sizeof(Native->StartupInfo));
      ZeroMemory(&Native->ProcessInfo, sizeof(Native->ProcessInfo));

      Native->StartupInfo.cb = sizeof(Native->StartupInfo);

      if (!CreateProcessA(NULL,
           const_cast<char*>(appName.data()),
           NULL,
           NULL,
           FALSE,
           CREATE_NO_WINDOW,
           NULL,
           const_cast<char*>(directory.data()),
           &Native->StartupInfo,
           &Native->ProcessInfo))
      {
         LOG_ERROR("Error in process creation: %s, error code: %d", appName.data(), GetLastError());
      }
   }

   ProcessHandle::~ProcessHandle()
   {
      TerminateProcess(Native->ProcessInfo.hProcess, 0);

      CloseHandle(Native->ProcessInfo.hProcess);
      CloseHandle(Native->ProcessInfo.hThread);
   }
}