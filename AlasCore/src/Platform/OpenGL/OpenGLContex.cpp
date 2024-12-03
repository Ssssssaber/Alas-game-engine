#include "OpenGLContext.h"

#include <glad/glad.h>
// #include <GL/GL.h>

namespace Hazel {

	// OpenGLContext::OpenGLContext(SDL_Window* windowHandle)
	// 	: _windowHandle(windowHandle)
	// {
	// 	AGS_ASSERT(windowHandle, "Window handle is null!")
	// }

	// void OpenGLContext::Init()
	// {
	// 	glfwMakeContextCurrent(_windowHandle);
	// 	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	// 	AGS_ASSERT(status, "Failed to initialize Glad!");

	// 	AGS_CORE_INFO("OpenGL Info:");
	// 	AGS_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
	// 	AGS_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
	// 	AGS_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

	// }

	// void OpenGLContext::SwapBuffers()
	// {
	// 	SDL_GL_SwapWindow(_window);
	// }

}