#include "platforms/declarations/process/process.h"

#include <Windows.h>

namespace platform
{
   struct ProcessHandle::NativeInfo
   {
      STARTUPINFO StartupInfo;
      PROCESS_INFORMATION ProcessInfo;
   };

   ProcessHandle::ProcessHandle(const std::string_view& appName)
   {
      Native = std::make_unique<NativeInfo>();

      ZeroMemory(&Native->StartupInfo, sizeof(Native->StartupInfo));
      ZeroMemory(&Native->ProcessInfo, sizeof(Native->ProcessInfo));

      Native->StartupInfo.cb = sizeof(Native->StartupInfo);
      
      CreateProcessA(NULL,
                     const_cast<char*>(appName.data()),
                     NULL,
                     NULL,
                     FALSE,
                     CREATE_NEW_CONSOLE,
                     NULL,
                     NULL,
                     &Native->StartupInfo,
                     &Native->ProcessInfo);
   }

   ProcessHandle::~ProcessHandle()
   {
      TerminateProcess(Native->ProcessInfo.hProcess, 0);

      CloseHandle(Native->ProcessInfo.hProcess);
      CloseHandle(Native->ProcessInfo.hThread);
   }
}