#include "http-handle.h"

#include <cstdint>
#include <string>

#include "debug/error.h"
#include "debug/log/log.h"

namespace net
{
   HttpHandle InitializeClientHTTP(const std::string_view& address, const uint16_t port)
   {
      WSAData wsaData;

      if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
      {
         err::CreateMBAndTerminate("Couldn't startup Winsock!");
         return {};
      }

      
      HttpHandle handle;

      handle.Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
      if(handle.Socket == INVALID_SOCKET)
      {
         err::CreateMBAndTerminate("Socket couldn't be created!");
         return {};
      }

      //uint8_t netAddres[32];
      //inet_pton(AF_INET, "127.0.0.1", netAddres);

      SOCKADDR_IN sockAddr;
      sockAddr.sin_port = htons(port);
      sockAddr.sin_family = AF_INET;
      sockAddr.sin_addr.S_un.S_addr = inet_addr(address.data());

      if(connect(handle.Socket, 
                 reinterpret_cast<SOCKADDR*>(&sockAddr),
                 sizeof(sockAddr)) == SOCKET_ERROR)
      {
         err::CreateMBAndTerminate("Couldn't connect to the server!");
         return {};
      }

      return handle;
   }

   void DestroyClientHTTP(const HttpHandle& handle)
   {
      if(closesocket(handle.Socket) == SOCKET_ERROR)
      {
         err::CreateMBAndTerminate("Couldn't close socket!");
         return;
      }

      if(WSACleanup() == SOCKET_ERROR)
      {
         err::CreateMBAndTerminate("Couldn't shutdown winsock!");
         return;
      };
   }

   void SendRequestHTTP(const HttpHandle& handle, const HttpRequest& requestInfo, const std::string_view& data)
   {
     std::string request = "POST / HTTP/1.1\r\n"
                           "Host: " + handle.Address
                           + "\r\nContent-Type:" + requestInfo.ContentType
                           + "\r\nContent-Length: " + std::to_string(requestInfo.ContentSize)
                           + "\r\n\r\n";

     if(send(handle.Socket, request.data(), request.length(), 0) == SOCKET_ERROR)
     {
        LOG_ERROR("Error sending http request! Winsock error: %d", WSAGetLastError());
        return;
     }

     if(send(handle.Socket, &data[0], requestInfo.ContentSize, 0) == SOCKET_ERROR)
        LOG_ERROR("Error in sending data with http request! Winsock error: %d", WSAGetLastError());
   }
}
