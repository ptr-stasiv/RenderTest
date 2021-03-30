#include "job-system.h"

#include <string>

#include "platforms/win64/win64-dev.h"

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

         SetThreadDescription(jobThread.native_handle(), (std::wstring(L"Job_thread_") + std::to_wstring(i)).c_str()); //Set thread name for debug purpose

         jobThread.detach();
      }
   }
}