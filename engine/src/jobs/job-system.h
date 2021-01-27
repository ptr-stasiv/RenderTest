#pragma once
#include <thread>
#include <list>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <any>

#include "utils/sync/spin-lock.h"

namespace core
{
   using JobFunc = void(*)(uintptr_t params);

   struct JobInfo
   {
      JobFunc    EntryPoint;
      uintptr_t  Params;
      uint16_t   InstanceCount;
   };

   class AsyncJobList
   {
   private:
      mutable std::list<JobInfo> List;
      mutable utils::sync::SpinLock SpinLock;

   public:
      inline void AddJob(const JobInfo& info)
      {
         std::lock_guard<utils::sync::SpinLock> l(SpinLock);
         List.emplace_back(info);
      }

      inline JobInfo GetNext() const
      {
         std::lock_guard<utils::sync::SpinLock> l(SpinLock);

         JobInfo job = List.front();
         List.pop_front();

         return job;
      }

      inline bool Empty() const
      {
         std::lock_guard<utils::sync::SpinLock> l(SpinLock);
         return List.empty();
      }
   };

   class JobSystem
   {
   private:
      static inline AsyncJobList JobList;

      static inline std::atomic_size_t FinishedJobCounter;
      static volatile inline size_t JobCounter;

      static inline std::condition_variable JobNotifyCV;
      static inline std::mutex Mutex;
   public:
      static void Setup();

      inline static void Execute(JobFunc func, uintptr_t params = 0)
      {
         std::lock_guard l(Mutex);

         JobList.AddJob({ func, params, 1 });

         ++JobCounter;

         JobNotifyCV.notify_one();
      }

      inline static void Wait()
      {
         while (FinishedJobCounter.load() < JobCounter);
      }
   };
}