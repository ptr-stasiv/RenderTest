#pragma once
#include "texture2d.h"

namespace graphics
{
	enum class CubeFace  : uint8_t
	{
		Top,
		Bottom,
		Front,
		Backward,
		Left,
		Right
	};

	class Cubemap
	{
	public:
		Cubemap() = default;
		virtual ~Cubemap() = default;

		virtual void InitData(const uint16_t sizeX, const uint16_t sizeY,
									 const InternalFormat internalFormat, const Format format,
									 const Type type, const TextureParams& params) = 0;

		virtual void UpdateFaceData(const CubeFace face, const void* data) = 0;

		//Through OpenGL api you should call this before init function
		virtual void SetBorderColor(const mm::vec4& color) const = 0;

		virtual uint16_t GetSizeX() const = 0;
		virtual uint16_t GetSizeY() const = 0;
	};
}