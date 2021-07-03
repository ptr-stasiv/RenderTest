#pragma once

#include "graphics/api/cubemap.h"
#include "graphics/api/texture2d.h"
#include "gl-api.h"
#include "GL/glew.h"

namespace graphics
{
	namespace gl
	{
		class CubemapGL : public Cubemap
		{
		public:
			GLuint64 Handle;
			GLuint BindId;
			uint16_t SizeX;
			uint16_t SizeY;

			Format CurrentFormat;
			Type CurrentType;

			mutable mm::vec4 BorderColor;
		
			CubemapGL() = default;

			~CubemapGL() override
			{
				glDeleteTextures(1, &BindId);
			}

			void InitData(const uint16_t sizeX, const uint16_t sizeY,
							  const InternalFormat internalFormat, const Format format,
							  const Type type, const TextureParams& params) override
			{
				SizeX = sizeX;
				SizeY = sizeY;

				CurrentFormat = format;
				CurrentType = type;

				glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &BindId);

				glTextureParameteri(BindId, GL_TEXTURE_WRAP_S, OGL_TEXTURE_WRAP(params.WrapS));
				glTextureParameteri(BindId, GL_TEXTURE_WRAP_T, OGL_TEXTURE_WRAP(params.WrapT));
				glTextureParameteri(BindId, GL_TEXTURE_MIN_FILTER, OGL_TEXTURE_FILTER(params.MinFilter));
				glTextureParameteri(BindId, GL_TEXTURE_MAG_FILTER, OGL_TEXTURE_FILTER(params.MagFilter));

				glTextureParameterfv(BindId, GL_TEXTURE_BORDER_COLOR, BorderColor.Data);

				glTextureStorage3D(BindId, 1, OGL_IFORMAT(internalFormat), sizeX, sizeY, 6);

				Handle = glGetTextureHandleARB(BindId);
				glMakeTextureHandleResidentARB(Handle);
			}

			void UpdateFaceData(const CubeFace face, const void* data) override
			{
				glTextureSubImage3D(BindId, 0, 0, 0, static_cast<GLint>(face), SizeX, SizeY, 1, OGL_FORMAT(CurrentFormat), OGL_TYPE(CurrentType), data);
			}

			void SetBorderColor(const mm::vec4& color) const override
			{
				BorderColor = color;
			}

			uint16_t GetSizeX() const override
			{
				return SizeX;
			}

			uint16_t GetSizeY() const override
			{
				return SizeY;
			}
		};
	}
}