#include "ResourceManager.h"

namespace Alas
{
    std::unordered_map<UID, std::string> ResourceManager::ResourcesFilapaths;    

    void ResourceManager::AddResource(UID id, const std::string& filepath)
    {
        ResourcesFilapaths[id] = filepath;
    }

    const std::string& ResourceManager::GetResourceFilepath(UID id)
    {
        return ResourcesFilapaths[id];
    }

} // namespace Alas
