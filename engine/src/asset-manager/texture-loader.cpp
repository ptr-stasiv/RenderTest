#include "texture-loader.h"
#include "stb/stb_image.h"

TextureData LoadTexture(const char* filepath)
{
   stbi_set_flip_vertically_on_load(1);
   
   TextureData resData;
   resData.Pixels = stbi_load(filepath, &resData.Width, &resData.Height, &resData.Channels, 0);

   return resData;
}