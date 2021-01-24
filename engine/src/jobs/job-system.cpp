#include "job-system.h"

namespace core
{
   void JobSystem::Setup()
   {
      uint32_t maxThreads = std::thread::hardware_concurrency();

      FinishedJobCounter.store(0);
      JobCounter = 0;

      for (uint32_t i = 0; i < maxThreads; ++i)
      {
         std::thread jobThread([]()
            {
               while (true)
               {
                  if (JobList.Empty())
                  {
                     std::unique_lock<std::mutex> l(Mutex);
                     JobNotifyCV.wait(l);
                  }

                  JobInfo job = JobList.GetNext();

                  for (uint16_t i = 0; i < job.InstanceCount; ++i)
                  {
                     job.EntryPoint(job.Params);
                  }

                  FinishedJobCounter.fetch_add(1);
               }
            });

         jobThread.detach();
      }
   }
}