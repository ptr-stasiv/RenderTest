#include "process.h"

#include "debug/log/log.h"
#include "debug/gassert.h"

namespace utils
{
   ProcessInfo CreateChildProcess(const std::string_view& execLocation, const std::string_view& workingDir)
   {
      HANDLE jobH = CreateJobObject(NULL, NULL);

      GASSERT(jobH, "Job couldn't created");

      JOBOBJECT_EXTENDED_LIMIT_INFORMATION jeli = { 0 };

      jeli.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;

      SetInformationJobObject(jobH, JobObjectExtendedLimitInformation, &jeli, sizeof(jeli));

      ProcessInfo info;

      ZeroMemory(&info.StartupInfo, sizeof(info.StartupInfo));
      ZeroMemory(&info.ProcessInfo, sizeof(info.ProcessInfo));

      info.StartupInfo.cb = sizeof(info.StartupInfo);

      GASSERT(CreateProcessA(NULL,
             const_cast<char*>(execLocation.data()),
             NULL,
             NULL,
             FALSE,
             CREATE_NO_WINDOW,
             NULL,
             const_cast<char*>(workingDir.data()),
             &info.StartupInfo,
             &info.ProcessInfo),
      "Error in process creation: %s, error code: %d", execLocation.data(), GetLastError());

      AssignProcessToJobObject(jobH, info.ProcessInfo.hProcess);

      return info;
   }

   void DestroyProcess(const ProcessInfo& processInfo)
   {
      TerminateProcess(processInfo.ProcessInfo.hProcess, 0);

      WaitForSingleObject(processInfo.ProcessInfo.hProcess, INFINITE);

      CloseHandle(processInfo.ProcessInfo.hProcess);
      CloseHandle(processInfo.ProcessInfo.hThread);
   }
}