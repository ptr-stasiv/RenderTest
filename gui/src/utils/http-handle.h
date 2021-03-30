#pragma once
#include <string_view>

#include "platforms/win64/win64-dev.h"

namespace net
{
   struct HttpHandle
   {
      SOCKET Socket;
      std::string Address;
      uint16_t Port;
   };

   struct HttpRequest
   {
      std::string_view Method;
      std::string_view ContentType;
      size_t ContentSize;
   };

   HttpHandle InitializeClientHTTP();

   void DestroyClientHTTP(const HttpHandle& handle);

   void SendRequestHTTP(const HttpHandle& handle, const HttpRequest& requestInfo, const std::string_view& data);
}
