#pragma once
#include <memory>

#include "window/window.h"
#include "graphics/render-manager.h"
#include "window/input/input-manager.h"

inline std::shared_ptr<graphics::GraphicsDevice> g_GraphicsDevice;

inline std::shared_ptr<app::Window> g_Window;

inline std::shared_ptr<graphics::RenderManager> g_RenderManager;

inline std::shared_ptr<input::InputManager> g_InputManager;