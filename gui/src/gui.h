#pragma once
#include <memory>
#include <cstdint>

namespace gui
{
   struct UlInfoPimpl;

   class GuiController
   {
   private:
      std::unique_ptr<UlInfoPimpl> UlInfo;
   public:
       GuiController();
      ~GuiController();

      void Setup(const uint32_t resX, const uint32_t resY);

      void GetRenderingInfo(uint32_t& resX, uint32_t& resY, void*& pixels);
   };
}