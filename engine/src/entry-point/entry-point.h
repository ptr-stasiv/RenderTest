#pragma once
#include <memory>
#include <functional>

#include "graphics/api/devices/gl-device.h"
#include "jobs/job-system.h"
#include "utils/timer.h"
#include "utils/config-file.h"

#include "global_systems.h"

#include "utils/config-file.h"

namespace app
{
   float g_DeltaTime;
   float g_FPS;

   inline void CreateEngineApp()
   {
      //Read engine config file
      auto updateFunc = [&](const utils::ConfigMap& map)
      {
         graphics::cfg::ShadowWidth = map.at("Shadow_Width").GetAsInt32();
         graphics::cfg::ShadowHeight = map.at("Shadow_Height").GetAsInt32();
         graphics::cfg::ShadowBias = map.at("Shadow_Bias").GetAsFloat();
      };

      utils::ConfigFile configFile("config.cef", updateFunc);


      //Resolving all subsystems dependencies
      
      g_GraphicsDevice = std::make_shared<graphics::OpenglGraphicsDevice>(); //TODO make the device selectable from the config if the will be one more

      g_Window = std::make_shared<app::Window>(g_GraphicsDevice);

      g_RenderManager = std::make_shared<graphics::RenderManager>(g_GraphicsDevice);

      g_DebugManager = std::make_shared<graphics::DebugPrimitiveManager>(g_RenderManager);

      g_InputManager = std::make_shared<input::InputManager>(g_Window);


      //Independent subsystems setup's
      
      core::JobSystem::Setup();
   }

   inline void RunEngineApp(const std::function<void()>& userTickFunc)
   {
     utils::Timer frameTimer;

     while(!g_Window->ShouldClose())
     {
        frameTimer.Reset();


        utils::ConfigFileUpdate();


        g_Window->BeginFrame();

        g_InputManager->Poll();

        userTickFunc();

        g_Window->EndFrame();


        g_DeltaTime = 1.0f / frameTimer.GetElapsedTime();
        g_FPS = 1000.0f / frameTimer.GetElapsedTime();
     }
   }
}
