#pragma once
#include <vector>

#include "event.h"
#include"debug/gassert.h"

namespace event
{
   using EventCallback = void(*)(BaseEvent&, const uintptr_t);

   class Subject
   {
   private:
      struct EventInfo
      {
         EventCallback Callback;
         uintptr_t Args;

         inline EventInfo(EventCallback callback, uintptr_t args)
            : Callback(callback), Args(args) {}
      };

      std::vector<EventInfo> ObserversArray;
   public:
      inline void Invoke(BaseEvent& e)
      {
         for (auto info : ObserversArray)
            info.Callback(e, info.Args);
      }

      inline size_t AddObserver(const EventCallback& callback, const uintptr_t args = 0)
      {
         ObserversArray.emplace_back(callback, args);
         return ObserversArray.size() - 1;
      }

      inline void RemoveObserver(const size_t id)
      {
         if(id < ObserversArray.size())
            ObserversArray.erase(ObserversArray.cbegin() + id);
      }
   };

   template<typename T>
   T CastEvent(BaseEvent& e)
   {
      GASSERT(e.GetType() == T::GetStaticType(), "Invalid event cast!");
      return static_cast<T&>(e);
   }
}