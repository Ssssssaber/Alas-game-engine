#include "Texture.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Resources/ResourceManager.h"

namespace Alas
{
    Shared<Texture> Texture::Create(const std::string& filepath, UID uid)
    {
        // create texture if not used
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:    ALAS_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:  return Shared<Texture>(new OpenGLTexture(uid, filepath));
            {
                Shared<Texture> texture = Shared<Texture>(new OpenGLTexture(uid, filepath));
                if (!ResourceManager::IsTextureUsed(uid))
                    ResourceManager::AddUsedResource(uid, texture);    
                return texture;
            }     
        }
        
        return nullptr;
    }
} // namespace Alas
