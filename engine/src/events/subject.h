#pragma once
#include <vector>

#include "event.h"
#include "debug/globals.h"

namespace event
{
   using CallbackFuncSignature = void(*)(BaseEvent&, const uintptr_t);

   struct Callback
   {
      CallbackFuncSignature Func;
      uintptr_t Args;

      inline Callback(const CallbackFuncSignature& func, const uintptr_t args = 0)
         : Func(func), Args(args) {}
   };

   class Subject
   {
   private:
      std::vector<Callback> ObserversArray;
   public:
      inline void Invoke(BaseEvent& e)
      {
         for (auto info : ObserversArray)
            info.Func(e, info.Args);
      }

      inline size_t AddObserver(const Callback& callback)
      {
         ObserversArray.emplace_back(callback);
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
      ASSERT(e.GetType() == T::GetStaticType(), "Invalid event cast!");
      return static_cast<T&>(e);
   }
}