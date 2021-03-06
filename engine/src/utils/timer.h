#pragma once
#include <Windows.h>
#include <memory>

namespace utils
{
   class Timer
   {
   private:
      struct NativeInfo;
      std::unique_ptr<NativeInfo> Native;
   public:
      Timer(const bool reset = false);
      ~Timer();

      void Reset();

      float GetElaspedTime() const;
   };
}