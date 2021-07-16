#pragma once
#include <memory>

#ifdef OPENGL
	#include "GL/glew.h"
	#include "platforms/opengl/gl-texture2d.h"
#endif

namespace editor
{
	class EditorManager
	{
	private:
#ifdef OPENGL
		GLuint ViewportPBO;
		std::unique_ptr<graphics::gl::Texture2dGL> ViewportTexture;
#endif
		void DrawPanels();
	public:
		EditorManager();

		void BeginScene();
		void EndScene();
	};
}