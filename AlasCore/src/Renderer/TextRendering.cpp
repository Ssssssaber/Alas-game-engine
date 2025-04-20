#include "TextRendering.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTextRendering.h"

namespace Alas
{
    Shared<TextRendering> TextRendering::Create()
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:    ALAS_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:  return Shared<TextRendering>(new OpenGLTextRendering());
        }

        ALAS_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
} // namespace Alas
