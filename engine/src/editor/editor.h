#pragma once
#include <memory>
#include <functional>

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

		uint16_t LastViewportW = 0;
		uint16_t LastViewportH = 0;
	public:
		std::function<void(const uint16_t, const uint16_t)> OnViewportResize;
		bool ViewportFocused;

		EditorManager();

		void SubmitViewport();
	};
}