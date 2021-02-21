#pragma once
#include <vector>

#include "event.h"
#include"debug/gassert.h"

namespace event
{
   using EventCallback = void(*)(BaseEvent&);

   class Subject
   {
   private:
      std::vector<EventCallback> ObserversArray;
   public:
      inline void Invoke(BaseEvent& e)
      {
         for (auto func : ObserversArray)
            func(e);
      }

      inline size_t AddObserver(const EventCallback& callback)
      {
         ObserversArray.push_back(callback);
         return ObserversArray.size() - 1;
      }

      inline void RemoveObserver(const size_t id)
      {
         if(id < ObserversArray.size())
            ObserversArray.erase(ObserversArray.cbegin() + id);
      }
   };

   template<typename T>
   T& CastEvent(BaseEvent& e)
   {
      GASSERT(e.GetType() == T::GetStaticType(), "Invalid event cast!");
      return static_cast<T>(e);
   }
}