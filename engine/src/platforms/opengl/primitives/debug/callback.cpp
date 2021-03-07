#include "callback.h"

#include "debug/log/log.h"

static const char* g_MessageSources[] =
{
   "OpenGL",
   "Window System",
   "Shader compiler",
   "Third party",
   "Application",
   "Other"
};

static const char* g_MessageType[] =
{
   "Error",
   "Deprecated",
   "Undefined",
   "Portability",
   "Performance",
   "Marker",
   "Group",
   "Group",
   "Other"
};

void GLAPIENTRY MessageCallbackOGL(GLenum source, GLenum type, GLuint id, GLenum severity,
   GLsizei length, const GLchar* message, const void* userParam)
{
   const char* messageSource = g_MessageSources[source - GL_DEBUG_SOURCE_API];
   const char* messageType = g_MessageType[type - GL_DEBUG_TYPE_ERROR];

   switch (severity)
   {
   case GL_DEBUG_SEVERITY_HIGH: 
      {
         WD_LOG_ERROR("[%s][%s] %s", messageSource, messageType, message);
      }break;
   case GL_DEBUG_SEVERITY_MEDIUM: 
      {
         WD_LOG_WARNING("[%s][%s] %s", messageSource, messageType, message);
      }break;
   case GL_DEBUG_SEVERITY_LOW: 
      {
         WD_LOG_WARNING("[%s][%s] %s", messageSource, messageType, message);
      }break;
   //case GL_DEBUG_SEVERITY_NOTIFICATION: 
   //   {
   //      WD_LOG_MESSAGE("[%s][%s] %s", messageSource, messageType, message);
   //   }break;
   default:
      break;
   }
}