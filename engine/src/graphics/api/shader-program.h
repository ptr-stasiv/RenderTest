#pragma once
#include <cstdint>
#include <sstream>

#include "math/matrices/matrix4x4.h"
#include "texture2d.h"
#include "uniform-buffer.h"
#include "shader-buffer.h"
#include "vertex-buffer.h"
#include "api.h"

namespace graphics
{
   enum class ShaderType : uint8_t
   {
      Vertex,
      Fragment,

      LAST_ENUM_ELEMENT
   };
   
   inline constexpr uint8_t ShadersTypeCount = static_cast<uint8_t>(ShaderType::LAST_ENUM_ELEMENT);

   class ShaderProgram
   {
   public:
      ShaderProgram() = default;
      virtual ~ShaderProgram() = default;

      virtual void AddShader(const ShaderType& type, const std::string_view& src) = 0;

      virtual void AddInputBuffer(const std::shared_ptr<VertexBuffer>& vbo, const uint8_t elements, const uint8_t attribIndex, 
                                  const size_t stride, const Type type, const size_t offset = 0, const size_t elementsOffset = 0) = 0;

      virtual void AddInputBuffer(const std::shared_ptr<UniformBuffer>& ubo, const std::string_view& name, const size_t dataSize, const size_t dataOffset = 0) = 0;

      virtual void AddInputBuffer(const std::shared_ptr<ShaderBuffer>& ssbo, const std::string_view& name) = 0;

      virtual void Compile() = 0;

      virtual void Use() const = 0;

      virtual void SetTexture2D(const std::string_view& name, const std::shared_ptr<graphics::Texture2D>& texture) const = 0;

      virtual void SetFloats(const std::string_view& name, const math::Matrix4& m) const = 0;

      virtual void SetFloats(const std::string_view& name, const math::Vector4& v) const = 0;

      virtual void SetFloats(const std::string_view& name, const math::Vector3& v) const = 0;

      virtual void SetFloat(const std::string_view& name, const float s) const = 0;

      virtual void SetInt(const std::string_view& name, const int32_t s) const = 0;
   };
}
