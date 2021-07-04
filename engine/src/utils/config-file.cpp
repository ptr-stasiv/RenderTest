#include "config-file.h"

#include <chrono>
#include <sstream>

#include "jobs/job-system.h"

namespace utils
{
   static std::vector<ConfigFile*> ConfigFilesArr;

   static std::map<std::string, ValueWrapper> ParseConfigFile(std::ifstream& file)
   {
      ConfigMap resMap;

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
      LastTimeModified = std::filesystem::last_write_time(path);

      ConfigFilesArr.push_back(this);
      Id = ConfigFilesArr.size() - 1;

      OpenFile();
      Callback();
      CloseFile();
   }

   ConfigFile::~ConfigFile()
   {
      ConfigFilesArr.erase(ConfigFilesArr.begin() + Id);
   }

   void ConfigFile::OpenFile()
   {
      FileHandler.open(Path);

      if (!FileHandler.is_open())
      {
         LOG_ERROR("Invalid config file path specified!");
         return;
      }
   }

   void ConfigFile::CloseFile()
   {
      FileHandler.close();
   }

   void ConfigFile::Callback()
   {
      try
      {
         OnFileUpdate(ParseConfigFile(FileHandler));
      }
      catch (...)
      {
         PRINT_AND_TERMINATE("Invalid config file!");
      }
   }

   void ConfigFileUpdate()
   {
      for (auto cf : ConfigFilesArr)
      {
         auto currentTime = std::filesystem::last_write_time(cf->Path);

         if (currentTime != cf->LastTimeModified)
         {
            cf->OpenFile();
            cf->Callback();
            cf->CloseFile();

            cf->LastTimeModified = currentTime;
         }
      }
   }
}
