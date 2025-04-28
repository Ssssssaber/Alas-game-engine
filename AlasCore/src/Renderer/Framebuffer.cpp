#include "Renderer/Framebuffer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"
#include "Renderer/Renderer.h"

namespace Alas {
	
	Shared<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    ALAS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return Shared<OpenGLFramebuffer>(new OpenGLFramebuffer(spec));
		}

		ALAS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}

