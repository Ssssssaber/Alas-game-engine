#include "RendererCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace AGS {
	RendererAPI* RenderCommand::_rendererAPI = new OpenGLRendererAPI;
}