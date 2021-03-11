#pragma once
#include <memory>
#include <functional>

#include "window/window.h"
#include "window/input/input-manager.h"
#include "gui/gui-manager.h"
#include "jobs/job-system.h"
#include "utils/timer.h"

namespace app
{
   std::shared_ptr<app::Window> g_Window;

   std::shared_ptr<input::InputManager> g_InputManager;

   std::shared_ptr<gui::GuiManager> g_GuiManager;

   float g_DeltaTime;
   float g_FPS;

   inline void CreateEngineApp()
   {
      //Resolving all subsystems dependencies

      g_Window = std::make_shared<app::Window>();

      g_InputManager = std::make_shared<input::InputManager>(g_Window);

      g_GuiManager = std::make_shared<gui::GuiManager>(g_Window);

      
      //Independent subsystems setup's
      
      core::JobSystem::Setup();
   }

   inline void RunEngineApp(const std::function<void()>& userTickFunc)
   {
     utils::Timer frameTimer;

     while(!g_Window->ShouldClose())
     {
        frameTimer.Reset();


        g_Window->BeginFrame();

        g_InputManager->Poll();


        userTickFunc();


        g_GuiManager->Update();

        g_Window->EndFrame();


        g_DeltaTime = 1.0f / frameTimer.GetElapsedTime();
        g_FPS = 1000.0f / frameTimer.GetElapsedTime();
     }
   }
}
