#pragma once
#include <atomic>

namespace utils
{
   namespace sync
   {
      class SpinLock
      {
      private:
         std::atomic_flag Flag = ATOMIC_FLAG_INIT;
      public:
         inline void lock()
         {
            while (Flag.test_and_set(std::memory_order_acquire));
         }

         inline void unlock()
         {
            Flag.clear(std::memory_order::memory_order_release);
         }
      };
   }
}