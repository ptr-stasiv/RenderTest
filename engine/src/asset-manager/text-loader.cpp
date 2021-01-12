#include "text-loader.h"

namespace assets
{
   char* ReadFromFile(const std::string_view filepath)
   {
      FILE* file = fopen(&filepath[0], "rb");
      if (!file)
         return NULL;

      fseek(file, 0, SEEK_END);
      uint64_t count = ftell(file);
      rewind(file);

      char* buffer = (char*)malloc(count + 1);
      if (!buffer)
         return NULL;

      fread(buffer, 1, count, file);
      buffer[count] = '\0';

      fclose(file);

      return buffer;
   }

   TextAssetData LoadText(const std::string_view filepath)
   {
      TextAssetData resData("");
      resData.Info.LoadTime = 0.0f;
      resData.Info.IsValid  = false;

      resData.TextStr = ReadFromFile(filepath);

      if (resData.TextStr.empty())
         return resData;

      resData.Info.LoadTime = 0.0f;
      resData.Info.IsValid  = true;

      return resData;
   }
}