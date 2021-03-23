#pragma once
#include <memory>
#include <cstdint>
#include <string>

#include "utils/process/process.h"

namespace gui
{
   struct UlInfoPimpl;

   class GuiController
   {
   private:
      std::unique_ptr<UlInfoPimpl> UlInfo;

      utils::ProcessInfo ServerProcess;
   public:
       GuiController();
      ~GuiController();

      void Setup(const uint32_t resX, const uint32_t resY);

      void GetRenderingInfo(uint32_t& resX, uint32_t& resY, void*& pixels);

      void OnMouseButton(const uint32_t button, const uint32_t state);
      void OnMouseMove(const float x, const float y);
      void OnMouseScroll(const float value);
   };
}