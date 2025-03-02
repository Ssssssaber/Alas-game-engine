#include "Texture.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Resources/ResourceManager.h"

namespace Alas
{
    Shared<Texture> Texture::Create(const std::string& filepath)
    {
        Shared<Texture> texture;
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:    ALAS_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:  texture = Shared<Texture>(new OpenGLTexture(filepath));
        }

        if (texture) 
        {
            ResourceManager::AddResource(texture->GetUID(), filepath);
            return texture;
        }
        
        ALAS_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
} // namespace Alas
