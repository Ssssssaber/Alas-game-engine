#include "RendererCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Alas {
	RendererAPI* RenderCommand::_rendererAPI = new OpenGLRendererAPI;
}