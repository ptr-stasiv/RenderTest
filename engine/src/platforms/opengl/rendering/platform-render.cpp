#include "graphics/renders/platform-render.h"

#include "GL/glew.h"
#include "debug/gassert.h"

namespace graphics
{
   struct PlatformRender::NativeInfo
   {
      GLuint Vao;
      
      GLuint PositionsVbo;
      GLuint NormalsVbo;
      GLuint UvsVbo;
   };

   PlatformRender::PlatformRender() = default;
   PlatformRender::~PlatformRender() = default;

   std::unique_ptr<PlatformRender> CreatePlatformRender()
   {
      PlatformRender* pr = new PlatformRender();
      pr->Native = std::make_unique<PlatformRender::NativeInfo>();

      //VAO setup

      glCreateVertexArrays(1, &pr->Native->Vao);

      glCreateBuffers(1, &pr->Native->PositionsVbo);
      glCreateBuffers(1, &pr->Native->NormalsVbo);
      glCreateBuffers(1, &pr->Native->UvsVbo);

      glNamedBufferData(pr->Native->PositionsVbo, sizeof(math::Vector3) * MaxVerticesPerDraw, nullptr, GL_DYNAMIC_DRAW);
      glNamedBufferData(pr->Native->NormalsVbo, sizeof(math::Vector3) * MaxVerticesPerDraw, nullptr, GL_DYNAMIC_DRAW);
      glNamedBufferData(pr->Native->UvsVbo, sizeof(math::Vector2) * MaxVerticesPerDraw, nullptr, GL_DYNAMIC_DRAW);

      glVertexArrayVertexBuffer(pr->Native->Vao, PositionAttribLocation, pr->Native->PositionsVbo, 0, sizeof(math::Vector3));
      glVertexArrayVertexBuffer(pr->Native->Vao, NormalAttribLocation, pr->Native->NormalsVbo, 0, sizeof(math::Vector3));
      glVertexArrayVertexBuffer(pr->Native->Vao, UvAttribLocation, pr->Native->UvsVbo, 0, sizeof(math::Vector3));

      glEnableVertexArrayAttrib(pr->Native->Vao, PositionAttribLocation);
      glEnableVertexArrayAttrib(pr->Native->Vao, NormalAttribLocation);
      glEnableVertexArrayAttrib(pr->Native->Vao, UvAttribLocation);

      glVertexArrayAttribBinding(pr->Native->Vao, PositionAttribLocation, PositionAttribLocation);
      glVertexArrayAttribBinding(pr->Native->Vao, NormalAttribLocation, NormalAttribLocation);
      glVertexArrayAttribBinding(pr->Native->Vao, UvAttribLocation, UvAttribLocation);

      glVertexArrayAttribFormat(pr->Native->Vao, PositionAttribLocation, 3, GL_FLOAT, GL_FALSE, 0);
      glVertexArrayAttribFormat(pr->Native->Vao, NormalAttribLocation, 3, GL_FLOAT, GL_FALSE, 0);
      glVertexArrayAttribFormat(pr->Native->Vao, UvAttribLocation, 2, GL_FLOAT, GL_FALSE, 0);

      struct alignas(16) PL
      {
         math::Vector4 Position;
         math::Vector4 Color;

         float Quadratic;
         float Linear;
         float Constant;
      };

      std::vector<PL> pl;
      pl.push_back(PL({ {1.0f, 0.2, 0.2f, 0.7f}, {0}, 0.5f, 0.0f, 1.0f }));
      pl.push_back(PL({ {0.2f, 0.2, 1.0f, 0.7f}, {0}, 0.5f, 0.0f, 1.0f }));
      pl.push_back(PL({ {0.2f, 1.0, 0.2f, 0.7f}, {0}, 0.5f, 0.0f, 1.0f }));

      //UBO's

      //glGenBuffers(1, &pr->Native->PointLightUBO);
      //glBindBuffer(GL_UNIFORM_BUFFER, pr->Native->PointLightUBO);
      //glBufferData(GL_UNIFORM_BUFFER, sizeof(PL) * pl.size(), pl.data(), GL_STATIC_DRAW);
      //glBindBufferBase(GL_UNIFORM_BUFFER, 0, pr->Native->PointLightUBO);

      return std::unique_ptr<PlatformRender>(pr);
   }

   void DestroyPlatformRender(const std::unique_ptr<PlatformRender>& pr)
   {
      glDeleteVertexArrays(1, &pr->Native->Vao);

      glDeleteBuffers(1, &pr->Native->PositionsVbo);
      glDeleteBuffers(1, &pr->Native->NormalsVbo);
      glDeleteBuffers(1, &pr->Native->UvsVbo);
   }

   void SetPlatformRenderMaterial(const std::unique_ptr<PlatformRender>& pr, const Material& material)
   {

   }

   void DrawPlatformRender(const std::unique_ptr<PlatformRender>& pr, const Mesh& mesh)
   {
      size_t size = mesh.Positions.size();

      GASSERT(size <= MaxVerticesPerDraw, "Too big draw call!");

      glNamedBufferSubData(pr->Native->PositionsVbo, 0, sizeof(math::Vector3) * size, &mesh.Positions[0]);
      glNamedBufferSubData(pr->Native->NormalsVbo, 0, sizeof(math::Vector3) * size, &mesh.Normals[0]);
      glNamedBufferSubData(pr->Native->UvsVbo, 0, sizeof(math::Vector2) * size, &mesh.UVs[0]);

      glBindVertexArray(pr->Native->Vao);
      glDrawArrays(GL_TRIANGLES, 0, size * 3);
   }

 //  void BatchDrawPlatformRender(const std::unique_ptr<PlatformRender>& pr, const std::vector<Mesh>& meshList)
 //  {

 //  }
}
