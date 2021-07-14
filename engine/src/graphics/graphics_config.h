#pragma once
#include <cstdint>

namespace graphics
{
	//This values will be updated through config file
	namespace cfg
	{
		inline uint16_t ShadowWidth = 512;
		inline uint16_t ShadowHeight = 512;

		inline float ShadowBias = 0.005f;
		inline float LightSize = 0.5f;
		inline int SoftShadows = 1;
	}
}