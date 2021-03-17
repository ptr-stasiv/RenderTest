#include "gl-canvas.h"

#include "debug/gassert.h"

namespace graphics
{
   namespace gl
   { 
         CanvasGL::CanvasGL(const uint16_t width, const uint16_t height, const std::string_view& title)
            : Width(width), Height(height)
         {
            GASSERT(glfwInit(), "Error in window creation");

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

            GlfwWindow = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);

            GASSERT(GlfwWindow, "Error in window creation");

            glfwMakeContextCurrent(GlfwWindow);

            GASSERT(glewInit() == GLEW_OK, "Error in window creation");


            //
            //This should be move out from here
            //

            const GLubyte* vendorInfo = glGetString(GL_VENDOR);
            const GLubyte* rendererInfo = glGetString(GL_RENDERER);
            const GLubyte* versionInfo = glGetString(GL_VERSION);
            const GLubyte* glslInfo = glGetString(GL_SHADING_LANGUAGE_VERSION);

            WD_LOG_MESSAGE("%s %s", vendorInfo, rendererInfo);
            WD_LOG_MESSAGE("Opengl %s", versionInfo);
            WD_LOG_MESSAGE("GLSL %s\n", glslInfo);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glBlendEquation(GL_FUNC_ADD);

            glEnable(GL_DEPTH_TEST);

            //glEnable(GL_DEBUG_OUTPUT);
            //glDebugMessageCallback(MessageCallbackOGL, 0);

            glfwSetInputMode(GlfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
         }
   }
}
