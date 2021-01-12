#include "image-loader.h"
#include "stb/stb_image.h"

namespace assets
{
   ImageAssetData LoadImage(const std::string_view filepath)
   {
      stbi_set_flip_vertically_on_load(1);

      ImageAssetData resData;
      resData.Info.LoadTime = 0.0f;
      resData.Info.IsValid  = false;

      resData.Pixels = stbi_load(&filepath[0], &resData.Width, &resData.Height, &resData.Channels, 0);

      if (!resData.Pixels)
         return resData;

      resData.Info.LoadTime = 0.0f;
      resData.Info.IsValid  = true;

      return resData;
   }
}