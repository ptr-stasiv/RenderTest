#include "utils/timer.h"


namespace utils
{
   struct Timer::NativeInfo
   {
      LARGE_INTEGER Frequency;
      LARGE_INTEGER StartTicks;
   };

   Timer::Timer(const bool reset)
   {
      Native = std::make_unique<NativeInfo>(NativeInfo({ 0 }));

      QueryPerformanceFrequency(&Native->Frequency);

      if (reset)
         Reset();
   }

   Timer::~Timer() = default;

   void Timer::Reset()
   {
      QueryPerformanceCounter(&Native->StartTicks);
   }

   float Timer::GetElaspedTime() const
   {
      LARGE_INTEGER endTicks;
      QueryPerformanceCounter(&endTicks);

      float diff = endTicks.QuadPart - Native->StartTicks.QuadPart;
      return diff / (Native->Frequency.QuadPart / 1000); //Division by 1000 is need to convert seconds to the milliseconds
   }
}