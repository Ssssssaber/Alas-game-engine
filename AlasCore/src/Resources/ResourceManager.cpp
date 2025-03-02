#include "ResourceManager.h"

namespace Alas
{
    std::unordered_map<UID, std::string> ResourceManager::ResourcesRegistry;  
    std::unordered_map<UID, Shared<Shader>> ResourceManager::UsedShaders;
    std::unordered_map<UID, Shared<Texture>> ResourceManager::UsedTextures;

    UID ResourceManager::GetResourceIdByPath(const std::string& filepath)
    {
        for (auto it = ResourcesRegistry.begin(); it != ResourcesRegistry.end(); ++it) 
        {
            if (it->second == filepath) return it->first;
        }

        return 0;
    }

    void ResourceManager::AddResourceToRegistry(UID id, const std::string& filepath)
    {
        if (ResourcesRegistry.find(id) != ResourcesRegistry.end())
        {
            ALAS_CORE_WARN(
                "Resource with id {0} already exists: {1}. Overriding with {2}: {3}",
                id, ResourcesRegistry[id], id, filepath);
        }

        ResourcesRegistry[id] = filepath;
    }

    const std::string& ResourceManager::GetResourceFilepath(UID id)
    {
        return ResourcesRegistry[id];
    }

    void ResourceManager::AddUsedResource(UID id, const Shared<Shader>& shader)
    {
        if (UsedShaders.find(id) != UsedShaders.end())
        {
            ALAS_CORE_WARN(
                "Resource with id {0} already exists. Overriding ...", id);
        }

        UsedShaders[id] = shader;
    }

    void ResourceManager::AddUsedResource(UID id, const Shared<Texture>& texture)
    {
        if (UsedShaders.find(id) != UsedShaders.end())
        {
            ALAS_CORE_WARN(
                "Resource with id {0} already exists. Overriding ...", id);
        }

        UsedTextures[id] = texture;
    }

    const Shared<Shader>& ResourceManager::GetUsedShader(UID id)
    {
        if (UsedShaders.find(id) == UsedShaders.end())
        {
            ALAS_CORE_WARN(
                "Resource with id {0} does not exist.", id);
            
            
        }

        return UsedShaders[id];
    }

    const Shared<Texture>& ResourceManager::GetUsedTexture(UID id)
    {
        if (UsedTextures.find(id) == UsedTextures.end())
        {
            ALAS_CORE_WARN(
                "Resource with id {0} does not exist. Using a default resource instead", id);
            
        }

        return UsedTextures[id];
    }

} // namespace Alas
