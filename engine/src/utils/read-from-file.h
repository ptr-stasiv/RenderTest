#pragma once
#include <cstdio>
#include <string_view>

namespace utils
{
   inline std::string_view ReadFromFile(const std::string_view& filepath)
   {
      FILE* file = fopen(filepath.data(), "rb");
      if (!file)
         return "";

      fseek(file, 0, SEEK_END);
      uint64_t count = ftell(file);
      rewind(file);

      char* buffer = (char*)malloc(count + 1);
      if (!buffer)
         return "";

      fread(buffer, 1, count, file);
      buffer[count] = '\0';

      fclose(file);

      return buffer;
   }
}