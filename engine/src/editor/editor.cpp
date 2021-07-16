#include "editor.h"

#include "entry-point/global_systems.h"

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
         graphics::InternalFormat::RGB8, graphics::Format::RGB, graphics::Type::Ubyte, vTexParams);

      glBindBuffer(GL_PIXEL_PACK_BUFFER, ViewportPBO);
      glBufferData(GL_PIXEL_PACK_BUFFER, 3 * g_Window->GetCanvas()->GetWidth() * g_Window->GetCanvas()->GetHeight(), nullptr, GL_STREAM_DRAW);
#endif
	}

   void EditorManager::BeginScene()
   {

   }

   void EditorManager::EndScene()
   {
#ifdef OPENGL
      glBindBuffer(GL_PIXEL_PACK_BUFFER, ViewportPBO);
      glReadPixels(0, 0, g_Window->GetCanvas()->GetWidth(), g_Window->GetCanvas()->GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, 0);

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
      static ImGuiID dockspaceID = 0;
      ImGui::Begin("Editor space", nullptr, ImGuiWindowFlags_MenuBar);

      dockspaceID = ImGui::GetID("EditorSpace");
      ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode);

      ImGui::End();


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

      ImGui::End();


      ImGui::Begin("Settings");
      ImGui::End();
   }
}