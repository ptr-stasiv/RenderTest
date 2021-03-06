#pragma once
#include <memory>

#include "window/window-wrapper.h"
#include "utils/timer.h"
#include "jobs/job-system.h"
#include "input/input-manager.h"
#include "gui/gui-wrapper.h"

namespace app
{
   class BaseApplication
   {
   public:
      std::shared_ptr<app::Window> Window;
      std::shared_ptr<input::InputManager> InputManager;

      std::shared_ptr<gui::GuiWrapper> Gui;

      float DeltaTime;

      inline BaseApplication()
      {
         Window = std::make_shared<app::Window>();
         InputManager = std::make_shared<input::InputManager>(new input::native::NativeInput(new input::InputWrapper(Window.get())));
         
         Gui = std::make_unique<gui::GuiWrapper>(Window->GetWidth(), Window->GetHeight());
         Gui->SetupInput(*InputManager);
      }

      virtual ~BaseApplication() {}

      virtual void OnStartup() {}
      virtual void OnTick() {}
   };

   extern BaseApplication* CreateApplication();
}

int main()
{
   auto app = std::shared_ptr<app::BaseApplication>(app::CreateApplication());

   core::JobSystem::Setup();

   app->OnStartup();

   utils::Timer deltaTimer;

   while (!app->Window->ShouldClose())
   {
      deltaTimer.Reset();

      app->Window->BeginFrame();
      app->InputManager->Poll();

      app->OnTick();
      app->Gui->Update();

      app->Window->EndFrame();

      app->DeltaTime = 1.0f / deltaTimer.GetElaspedTime();
   }

   return 0;
}