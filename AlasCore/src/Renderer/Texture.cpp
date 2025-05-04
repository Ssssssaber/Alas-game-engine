#include "Texture.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Resources/ResourceManager.h"

namespace Alas
{
    Shared<Texture> Texture::Create(const std::string& filepath)
    {
        UID textureID = ResourceManager::RecourceExists(filepath);
        Shared<Texture> texture;
        // check is filepath is in registry
        if (!textureID)
        {
            ResourceManager::UpdateMetaFiles();
            textureID = ResourceManager::RecourceExists(filepath);
            if (!textureID)
            {
                ALAS_ASSERT(false, "Recource manager does not have info about recource {0}");
                return texture;
            }

        }

        texture = ResourceManager::IsTextureUsed(textureID);
        // return shader if already used
        if (texture)
        {
            return texture;
        }

        // create texture if not used
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:    ALAS_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:  texture = Shared<Texture>(new OpenGLTexture(textureID, filepath));
        }
        
        if (texture) 
        {
            ResourceManager::AddUsedResource(textureID, texture);
        }
    
        return texture;
    }
} // namespace Alas
