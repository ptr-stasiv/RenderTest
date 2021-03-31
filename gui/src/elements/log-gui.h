#pragma once
#include <memory>

#include "gui-manager.h"

namespace gui
{
   size_t GetElementId()
   {
      static size_t id = 0;
      return id++;
   }

   class Log
   {
   private:
      std::shared_ptr<GuiManager> GuiM;
      size_t Id;
   public:
      inline Log(const std::shared_ptr<GuiManager>& guiM)
         : GuiM(guiM), Id(GetElementId())
      {
         GuiM->AddNewElementXML({ "log", { { "id", std::to_string(Id) } }, "" });
      }

      inline ~Log()
      {
         GuiM->AddRemoveElementXML({ "log", { { "id", std::to_string(Id) } }, "" });
      }

      inline void AddToLog(const std::string_view& src)
      {

      }
   };
}