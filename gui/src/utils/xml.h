#pragma once
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>

#include "debug/gassert.h"

namespace utils
{
   //Simple XML generator don't have much functionality but as with JSON generator I don't need more for it purpose

   struct XmlTag
   {
      std::string Name;

      std::string Value;

      std::vector<std::pair<std::string, std::string>> Attributes;

      std::unordered_map<std::string, XmlTag> NestedTags;
      
      XmlTag() = default;

      inline XmlTag(const std::string_view& name, 
                    const std::initializer_list<std::pair<std::string, std::string>>& attributes,
                    const std::string_view& value) 
         : Name(name), Value(value), Attributes(attributes)  {}
      
      inline XmlTag(const std::string_view& name,
                    const std::initializer_list<std::pair<std::string, std::string>>& attributes,
                    const std::initializer_list<XmlTag>& nestedTags)
         : Name(name), Attributes(attributes) 
      {
         for (auto& t : nestedTags)
            NestedTags[t.Name] = t;
      }

      inline void operator = (const XmlTag& tag)
      {
         NestedTags.insert({ tag.Name, tag });
      }

      std::string ToString() const
      {
         std::string resString;

         resString = '<' + Name;

         for (auto& a : Attributes)
         {
            auto[name, value] = a;
            
            resString += ' ';
            resString += name;
            resString += '=';
            resString += value;
         }

         resString += '>';

         resString += Value;

         for (auto& t : NestedTags)
         {
            auto [key, value] = t;
            resString += value.ToString();
         }

         resString += "</" + Name + '>';

         return resString;
      }
   };

   class Xml
   {
   private:
      std::unordered_map<std::string, XmlTag> TagMap;
   public:
      inline void Add(const XmlTag& tag, const std::string& root = "")
      {
         if(root == "")
            TagMap.insert({ tag.Name, tag });
         else
         {
            GASSERT(TagMap.find(root) != TagMap.end(), "Invalid root tag passed");
            TagMap[root].NestedTags.insert({ tag.Name, tag });
         }
      }

      inline void Add(const XmlTag& tag, const std::initializer_list<std::string> &rootH)
      {
         std::unordered_map<std::string, XmlTag>::iterator curTag;
         size_t i = 0;
         for(auto& l : rootH)
         {
            if(i == 0)
            {
               auto& f = TagMap.find(l);
               GASSERT(f != TagMap.end(), "Invalid root tag passed");

               curTag = f;

               continue;
            }
            
            auto& f = curTag->second.NestedTags.find(l);
            GASSERT(f != curTag->second.NestedTags.end(), "Invalid tag path passed!");

            curTag = f;

            ++i;
         }

         curTag->second.NestedTags.insert({ tag.Name, tag });
      }

      inline XmlTag& operator [] (const std::string& rootTag)
      {
         GASSERT(TagMap.find(rootTag) != TagMap.end(), "Invalid tag name passed");
         return TagMap[rootTag];
      }

      inline std::string ToString() const
      {
         std::string resString;

         for(auto& t : TagMap)
         {
            auto[key, value] = t;
            resString += value.ToString();
         }

         return resString;
      }
   };
}
