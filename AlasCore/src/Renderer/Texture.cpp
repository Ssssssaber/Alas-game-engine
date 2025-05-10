#include "Texture.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"
// #include "Resources/ResourceManager.h"

namespace Alas
{
    Shared<Texture> Texture::Create(const std::string& filepath, UID uid)
    {
        // create texture if not used
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:    ALAS_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:
            {
                Shared<Texture> texture = Shared<Texture>(new OpenGLTexture(uid, filepath));
                // if (!ResourceManager::IsTextureUsed(uid))
                //     ResourceManager::AddUsedResource(uid, texture);    
                return texture;
            }     
        }
        
        return nullptr;
    }

    Shared<Texture> Texture::Create(UID uid, uint32_t width, uint32_t height)
    {
        // create texture if not used
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:    ALAS_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:
            {
                Shared<Texture> texture = Shared<Texture>(new OpenGLTexture(uid, width, height));
                // if (!ResourceManager::IsTextureUsed(uid))
                //     ResourceManager::AddUsedResource(uid, texture);    
                return texture;
            }     
        }
        
        return nullptr;
    }
} // namespace Alas
