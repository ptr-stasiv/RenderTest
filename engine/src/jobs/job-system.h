#pragma once
#include <thread>
#include <list>
#include <mutex>
#include <condition_variable>
#include <functional>

namespace core
{
   using JobFunc = void(*)();

   struct JobInfo
   {
      JobFunc  EntryPoint;
      uint16_t InstanceCount;
   };

   class AsyncJobList
   {
   private:
      mutable std::list<JobInfo> List;
      mutable std::mutex Mutex;

   public:
      inline void AddJob(const JobInfo& info)
      {
         std::lock_guard lock(Mutex);
         List.emplace_back(info);
      }

      inline JobInfo GetNext() const
      {
         std::lock_guard l(Mutex);

         JobInfo job = List.front();
         List.pop_front();

         return job;
      }

      inline bool Empty() const
      {
         std::lock_guard l(Mutex);
         return List.empty();
      }
   };

   class JobSystem
   {
   private:
      static inline AsyncJobList JobList;

      static inline std::condition_variable JobNotifyCV;
      static inline std::mutex Mutex;

      static inline std::atomic_size_t FinishedJobCounter;
      static volatile inline size_t JobCounter;
   public:
      static void Setup();

      inline static void Execute(JobFunc func)
      {
         std::lock_guard l(Mutex);

         JobList.AddJob({ func, 1 });

         ++JobCounter;

         JobNotifyCV.notify_one();
      }

      inline static void Wait()
      {
         while (FinishedJobCounter.load() < JobCounter);
      }
   };
}