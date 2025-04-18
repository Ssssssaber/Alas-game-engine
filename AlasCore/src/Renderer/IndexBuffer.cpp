#include "IndexBuffer.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLIndexBuffer.h"

namespace Alas {

    IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    ALAS_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return new OpenGLIndexBuffer(indices, size);
		}

		ALAS_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}