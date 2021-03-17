#include "forward-render.h"

#include "utils/read-from-file.h"

namespace graphics
{
   constexpr std::string_view VertexShaderLocation = "res/shaders/phong-shading.vs";
   constexpr std::string_view FragmentShaderLocation = "res/shaders/phong-shading.fs";

   ForwardRender::ForwardRender()
   {  
      /*MainShader = std::make_unique<ShaderProgram>();
      MainShader->Add(ShaderType::Vertex, utils::ReadFromFile(VertexShaderLocation));
      MainShader->Add(ShaderType::Fragment, utils::ReadFromFile(FragmentShaderLocation));
      MainShader->Compile();*/

      CurrentRender = CreatePlatformRender();
   }

   ForwardRender::~ForwardRender()
   {
      DestroyPlatformRender(CurrentRender);
   }

   void ForwardRender::Render(const Camera& camera)
   {
      /*MainShader->Use();

      MainShader->SetFloats("projection", camera.GetCameraProjection());
      MainShader->SetFloats("view", camera.GetCameraViewMatrix());

      for(auto r : RendererList)
      { 
         MainShader->SetFloats("model", r.Transformation);

         SetPlatformRenderMaterial(CurrentRender, r.Material);
         DrawPlatformRender(CurrentRender, r.Mesh);
      }*/
   }
}
