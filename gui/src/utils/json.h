#pragma once
#include <vector>
#include <cstdint>
#include <string_view>
#include <string>

namespace utils
{
   //Simple class to generate JSON text
   //Not have very much functionality and I think not very fast 
   //But its will be used only to generate small server requests and current functionality is enough
   
   struct ElementPair
   {
      std::string Key = "";
      std::string Value = "";

      std::vector<ElementPair> NestedPairs;

      bool ValueString = false;

      inline ElementPair() = default;

      inline ElementPair(const std::string_view& key) 
         : Key(key){}

      inline ElementPair(const std::string_view& key,
                         const std::initializer_list<ElementPair>& nestedPairs)
         : Key(key), NestedPairs(nestedPairs) {}

      inline ElementPair(const std::string_view& key,
                         ElementPair&& nestedPair)
         : Key(key) 
      {
         NestedPairs.emplace_back(nestedPair);
      }

      inline ElementPair(const std::string_view& key,
                         const char* value)
         : Key(key), Value(value), ValueString(true) {}

      template<typename T>
      inline ElementPair(const std::string_view& key,
                         const T& value)
         : Key(key), Value(std::to_string(value)) {}

      inline void operator = (const char* value)
      {
         Value = value;
         ValueString = true;
      }

      template<typename T>
      inline void operator = (const T& value)
      {
         Value = std::to_string(value);
      } 

      inline void operator = (const std::initializer_list<ElementPair>& nestedPairs)
      {
         NestedPairs = nestedPairs;
      }

      inline const std::string GetMerged() const
      {
         std::string resString;
         
         resString  = "\"";

         resString += Key;

         resString += "\": ";


         if (NestedPairs.empty())
         {
            if (ValueString)
               resString += "\"";

            resString += Value;

            if (ValueString)
               resString += "\"";

            return resString;
         }

         resString += "{";

         for (size_t i = 0; i < NestedPairs.size(); ++i)
         {
            resString += NestedPairs[i].GetMerged();

            if(i < NestedPairs.size() - 1)
               resString += ',';
         }

         resString += "}";

         return resString;
      }
   };

   class Json
   {
   private:
      std::vector<ElementPair> PairList;
   public:
      inline ElementPair& operator [] (const std::string_view& key)
      {
         PairList.emplace_back(key);
         return PairList[PairList.size() - 1];
      }

      inline const std::string ToString() const
      {
         std::string resString;

         resString += '{';

         for(size_t i = 0; i < PairList.size(); ++i)
         {
            resString += PairList[i].GetMerged();

            if(i < PairList.size() - 1)
               resString += ',';
         }

         resString += '}';
         
         return resString;
      }
   };
}
