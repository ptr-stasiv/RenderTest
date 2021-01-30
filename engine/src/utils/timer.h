#pragma once
#include <Windows.h>
#include <cstdint>

namespace utils
{
   class Timer
   {
   private:
      LARGE_INTEGER Frequency;
      LARGE_INTEGER StartTicks;
   public:
      inline Timer(const bool reset = false)
         : StartTicks({ 0 })
      {
         QueryPerformanceFrequency(&Frequency);

         if (reset)
            Reset();
      }

      inline void Reset()
      {
         QueryPerformanceCounter(&StartTicks);
      }

      inline float GetElaspedTime() const
      {
         LARGE_INTEGER endTicks;
         QueryPerformanceCounter(&endTicks);

         float diff = endTicks.QuadPart - StartTicks.QuadPart;
         return diff / (Frequency.QuadPart / 1000); //Division by 1000 is need to convert seconds to the milliseconds
      }
   };
}