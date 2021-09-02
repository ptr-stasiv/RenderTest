#include "editor.h"

#include "entry-point/global_systems.h"
#include "graphics/graphics_config.h"

namespace editor
{
	EditorManager::EditorManager()
	{
#ifdef OPENGL
      glGenBuffers(1, &ViewportPBO);
      glBindBuffer(GL_PIXEL_PACK_BUFFER, ViewportPBO);

      graphics::TextureParams vTexParams;
      vTexParams.MagFilter = graphics::TextureFilter::Linear;
      vTexParams.MinFilter = graphics::TextureFilter::Linear;
      vTexParams.WrapS = graphics::TextureWrap::ClampToEdge;
      vTexParams.WrapT = graphics::TextureWrap::ClampToEdge;

      ViewportTexture.reset();
      ViewportTexture = std::make_unique<graphics::gl::Texture2dGL>();
      ViewportTexture->InitData(g_Window->GetCanvas()->GetWidth(), g_Window->GetCanvas()->GetHeight(),
         graphics::InternalFormat::RGB32F, graphics::Format::RGB, graphics::Type::Float, vTexParams);

      glBindBuffer(GL_PIXEL_PACK_BUFFER, ViewportPBO);
      glBufferData(GL_PIXEL_PACK_BUFFER, 3 * sizeof(float) * g_Window->GetCanvas()->GetWidth() * g_Window->GetCanvas()->GetHeight(), nullptr, GL_STREAM_DRAW);
#endif
	}

   void EditorManager::SubmitViewport()
   {
#ifdef OPENGL
      glBindBuffer(GL_PIXEL_PACK_BUFFER, ViewportPBO);
      glReadPixels(0, 0, g_Window->GetCanvas()->GetWidth(), g_Window->GetCanvas()->GetHeight(), GL_RGB, GL_FLOAT, 0);

      GLubyte* ptr = (GLubyte*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);

      if (ptr)
         ViewportTexture->UpdateData(g_Window->GetCanvas()->GetWidth(), g_Window->GetCanvas()->GetHeight(), ptr);

      glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
#endif

      g_GraphicsDevice->Clear();

      DrawPanels();
   }

   void EditorManager::DrawPanels()
   {
      //Set dock space size to window
      ImGui::SetNextWindowSize(ImVec2(g_Window->GetCanvas()->GetWidth(),
                                      g_Window->GetCanvas()->GetHeight()));

      static ImGuiID dockspaceID = 0;
      ImGui::Begin("Editor space", nullptr, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

      dockspaceID = ImGui::GetID("EditorSpace");
      ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode);

      ImGui::End();


      //Set vieport limit
      ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(g_Window->GetCanvas()->GetWidth(), 
                                                               g_Window->GetCanvas()->GetHeight()));

      ImGui::Begin("Viewport");

#ifdef OPENGL
      ImVec2 vSize = ImGui::GetContentRegionAvail();

      if (vSize.x > g_Window->GetCanvas()->GetWidth())
         vSize.x = g_Window->GetCanvas()->GetWidth();
      if (vSize.y > g_Window->GetCanvas()->GetHeight())
         vSize.y = g_Window->GetCanvas()->GetHeight();
      
      glBindTexture(GL_TEXTURE_2D, ViewportTexture->BindId);

      ImGui::Image((void*)ViewportTexture->BindId, 
                   ImVec2(vSize.x, vSize.y),
                   ImVec2(0, 1), ImVec2(1, 0));
#endif

      if (vSize.x != LastViewportW || vSize.y != LastViewportH)
      {
         LastViewportW = vSize.x;
         LastViewportH = vSize.y;

         if (OnViewportResize)
            OnViewportResize(vSize.x, vSize.y);
      }

      ViewportFocused = ImGui::IsWindowFocused();


      ImGui::End();


      ImGui::Begin("Settings");


      float bias = graphics::cfg::ShadowBias;
      float lightSize = graphics::cfg::LightSize;
      bool softShadows = graphics::cfg::SoftShadows;

      ImGui::Text("Shadows");
      ImGui::SliderFloat("Bias", &bias, 0.005f, 0.01f);
      ImGui::SliderFloat("LightSize", &lightSize, 0.0f, 1.0f);
      ImGui::Checkbox("Soft shadows", &softShadows);

      graphics::cfg::ShadowBias = bias;
      graphics::cfg::LightSize = lightSize;
      graphics::cfg::SoftShadows = softShadows;

      ImGui::End();
   }
}