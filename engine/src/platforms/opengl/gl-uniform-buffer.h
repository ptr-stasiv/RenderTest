#pragma once
#include "graphics/api/uniform-buffer.h"
#include "gl-shader.h"
#include "GL/glew.h"

namespace graphics
{
   namespace gl
   {
      class UniformBufferGL : public UniformBuffer
      {
      private:
         uint8_t BlockCounter = 0;
      public:
         GLuint BindId;

         UniformBufferGL()
         {
            glGenBuffers(1, &BindId);
         }

         ~UniformBufferGL() override
         {
            glDeleteBuffers(1, &BindId);
         }

         inline void InitData(const size_t size, const void* data) override
         {
            glBindBuffer(GL_UNIFORM_BUFFER, BindId);
            glBufferData(GL_UNIFORM_BUFFER, size, data, GL_DYNAMIC_DRAW);
         }

         inline void UpdateData(const size_t size, const void* data, const size_t offset = 0) override
         {
            glBindBuffer(GL_UNIFORM_BUFFER, BindId);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);
         }

         inline void BindBlock(const std::shared_ptr<ShaderProgram>& shader, const std::string_view& name, const size_t dataSize, const size_t dataOffset = 0) override
         {
            auto& glShader = std::static_pointer_cast<ShaderProgramGL>(shader);

            GLuint blockId = glGetUniformBlockIndex(glShader->ProgramId, &name[0]);

            glUniformBlockBinding(glShader->ProgramId, blockId, BlockCounter);

            glBindBuffer(GL_UNIFORM_BUFFER, BindId);
            glBindBufferRange(GL_UNIFORM_BUFFER, BlockCounter, BindId, 0, dataSize);

            ++BlockCounter;
         }
      };
   }
}
