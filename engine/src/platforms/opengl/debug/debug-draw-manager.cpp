#include "debug/graphics/debug-draw-manager.h"

#include "platforms/opengl/primitives/buffers/vertex-array.h"
#include "math/math_utils.h"

namespace debug
{
   struct DebugDrawManager::DrawInfo
   {
      gl::VertexArray Vao;
      size_t VerticesCount;

      DrawInfo(const gl::VertexArray& vao, const size_t vc)
         : Vao(vao), VerticesCount(vc) {}
   };

   DebugDrawManager::DebugDrawManager()
   {
      auto* vertexShaderSrc = R"(
            #version 460 core
      
            layout(location = 0) in vec3 pos;

            uniform mat4 view = mat4(1.0f);
            uniform mat4 model = mat4(1.0f);
            uniform mat4 projection = mat4(1.0f);

            void main()
            {
               gl_Position = projection * model * view * vec4(pos, 1.0f);
            })";

      auto* fragmentShaderSrc = R"(
            #version 460 core
         
            out vec4 Color;

            void main()
            {
               Color = vec4(1.0f, 0.2f, 0.2f, 1.0f);
            })";


      PrimitiveShader.Add(graphics::ShaderType::Vertex, vertexShaderSrc);
      PrimitiveShader.Add(graphics::ShaderType::Fragment, fragmentShaderSrc);

      PrimitiveShader.Compile();
   }
   
   DebugDrawManager::~DebugDrawManager() = default;


   void DebugDrawManager::AddDebugSphere(const math::Vector3& center, const float radius, const uint8_t sectors, const uint8_t stacks)
   {
      const float vStep = math::Pi / stacks;
      const float hStep = math::Pi2 / sectors;

      std::vector<math::Vector3> vertices;

      for (size_t stack = 0; stack <= stacks; ++stack)
      {
         float stackStep = math::Pi / 2 - vStep * stack;
         float nextStackStep = math::Pi / 2 - vStep * (stack + 1);
            
         for (size_t sector = 0; sector <= sectors; ++sector)
         {
            float sectorStep = hStep * sector;
            float nextSectorStep = hStep * (sector + 1);

            float x = radius * cos(sectorStep) * cos(stackStep);
            float y = radius * sin(stackStep);
            float z = radius* sin(sectorStep)* cos(stackStep);

            math::Vector3 v1(x, y, z);


            x = radius * cos(nextSectorStep) * cos(stackStep);
            y = radius * sin(stackStep);
            z = radius * sin(nextSectorStep) * cos(stackStep);

            math::Vector3 v2(x, y, z);


            x = radius * cos(nextSectorStep) * cos(nextStackStep);
            y = radius * sin(nextStackStep);
            z = radius * sin(nextSectorStep) * cos(nextStackStep);

            math::Vector3 v3(x, y, z);
           

            vertices.push_back(v1);
            vertices.push_back(v2);
            vertices.push_back(v3);
         }
      }

      gl::VertexArray vao = gl::CreateVertexArray();

      gl::VertexBuffer vbo = gl::CreateVertexBuffer(vertices.size() * 3 * sizeof(float), vertices.data());
      GLuint b = gl::AddBufferVertexArray(vao, vbo, 3 * sizeof(float));
      gl::AddAttribFormatVertexArray(vao, 0, b, 3, GL_FLOAT, GL_FALSE, 0);

      DrawPrimitivesArray.emplace_back(std::make_unique<DrawInfo>(vao, vertices.size() * 3));
   }

   void DebugDrawManager::Draw(const graphics::Camera& camera)
   {
      PrimitiveShader.Use();
      PrimitiveShader.SetFloats("projection", camera.GetCameraProjection());
      PrimitiveShader.SetFloats("view", camera.GetCameraViewMatrix());

      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

      for(size_t i = 0; i < DrawPrimitivesArray.size(); ++i)
      {
         gl::BindVertexArray(DrawPrimitivesArray[i]->Vao);
         glDrawArrays(GL_TRIANGLES, 0, DrawPrimitivesArray[i]->VerticesCount);
      }
      
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   }
}
