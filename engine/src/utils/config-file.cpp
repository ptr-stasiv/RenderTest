#include "config-file.h"

#include <chrono>
#include <sstream>

namespace utils
{
   static std::map<std::string, ValueWrapper> ParseConfigFile(std::ifstream& file)
   {
      std::map<std::string, ValueWrapper> resMap;

      std::string line;
      while(std::getline(file, line))
      {
         std::stringstream ss(line);
         
         std::string key;
         std::getline(ss, key, ':');

         std::string value;
         std::getline(ss, value);

         if(!(key.empty() && value.empty()))
            resMap[key] = value; 
      };

      return resMap;
   }

   ConfigFile::ConfigFile(const std::string& path, const std::function<void(const ConfigMap&)> updateFunc)
      : Path(path), OnFileUpdate(updateFunc)
   { 
      OnFileUpdate(ParseConfigFile(FileHandler));
   }
}
