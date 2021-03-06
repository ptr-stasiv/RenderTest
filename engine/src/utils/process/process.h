#pragma once
#include <sstream>
#include <memory>

namespace utils
{
   class ProcessHandle
   {
   private:
      struct NativeInfo;
      std::unique_ptr<NativeInfo> Native;
   public:
      ProcessHandle(const std::string_view& appName, const std::string_view& directory);
      ~ProcessHandle();

      ProcessHandle(const ProcessHandle*) = delete;
      ProcessHandle(ProcessHandle&&) = delete;
      ProcessHandle& operator = (const ProcessHandle&) = delete;
      ProcessHandle& operator = (ProcessHandle&&) = delete;
   };
}