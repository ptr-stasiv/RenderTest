#include "process.h"

#include "debug/globals.h"
#include "debug/globals.h"

namespace utils
{
   ProcessInfo CreateChildProcess(const std::string_view& execLocation, const std::string_view& workingDir)
   {
      ProcessInfo info;

      info.JobHandle = CreateJobObject(NULL, NULL);

      ASSERT(info.JobHandle, "Job couldn't created");

      JOBOBJECT_EXTENDED_LIMIT_INFORMATION jeli = { 0 };

      jeli.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;

      SetInformationJobObject(info.JobHandle, JobObjectExtendedLimitInformation, &jeli, sizeof(jeli));

      ZeroMemory(&info.StartupInfo, sizeof(info.StartupInfo));
      ZeroMemory(&info.ProcessInfo, sizeof(info.ProcessInfo));

      info.StartupInfo.cb = sizeof(info.StartupInfo);

      ASSERT(CreateProcessA(NULL,
             const_cast<char*>(execLocation.data()),
             NULL,
             NULL,
             FALSE,
             CREATE_NO_WINDOW,
             NULL,
             const_cast<char*>(workingDir.data()),
             &info.StartupInfo,
             &info.ProcessInfo),
      "Error in process creation!");

      AssignProcessToJobObject(info.JobHandle, info.ProcessInfo.hProcess);

      return info;
   }

   void DestroyProcess(const ProcessInfo& processInfo)
   {
      CloseHandle(processInfo.JobHandle);

      CloseHandle(processInfo.ProcessInfo.hProcess);
      CloseHandle(processInfo.ProcessInfo.hThread);
   }
}