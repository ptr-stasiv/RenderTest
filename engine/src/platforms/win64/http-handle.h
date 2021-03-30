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
      std::string Method;
      std::string ContentType;
      size_t ContentSize;
   };

   HttpHandle InitializeClientHTTP(const std::string_view& address, const uint16_t port, bool connectLoop = false); //Connect loop mean that it will try to connect until it will be succeed

   void DestroyClientHTTP(const HttpHandle& handle);

   void SendRequestHTTP(const HttpHandle& handle, const HttpRequest& requestInfo, const std::string_view& data);
}
