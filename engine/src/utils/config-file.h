#pragma once

#include <string>
#include <map>
#include <functional>
#include <stdexcept>
#include <fstream>
#include <filesystem>

#include "debug/globals.h"

namespace utils
{
   class ValueWrapper
   {
   private:
      std::string Value;
   public:
      ValueWrapper() = default;

      inline ValueWrapper(const std::string& str)
         : Value(str) {}

      inline std::string GetAsString() const
      {
         return Value;
      }

      inline float GetAsFloat() const
      {
         float res = 0.0f;

         try
         {
            res = std::stof(Value);
         }
         catch(const std::invalid_argument& e)
         {
            PRINT_AND_TERMINATE("Value couldn't be converted to float!");
         }

         return res;
      }

      inline int32_t GetAsInt32() const
      {
         int32_t res = 0;
         
         try
         {
            res = std::stoi(Value);
         }
         catch(const std::invalid_argument& e)
         {
            PRINT_AND_TERMINATE("Value couldn't be converted to int32!");
         }

         return res;
      }
   };

   //This function is called in the engine main loop
   void ConfigFileUpdate();

   using ConfigMap = std::map<std::string, ValueWrapper>;

   struct ConfigFile
   {
   private:
      std::filesystem::path Path;

      std::ifstream FileHandler;
        
      //This function is called when the file opens first time and later when the file modify
      std::function<void(const ConfigMap&)> OnFileUpdate;

      std::filesystem::file_time_type LastTimeModified;

      size_t Id;

      void OpenFile();
      void CloseFile();

      friend void ConfigFileUpdate();
   public:
      ConfigFile(const std::string& path, const std::function<void(const ConfigMap&)> updateFunc);
      ~ConfigFile();
   };
}
