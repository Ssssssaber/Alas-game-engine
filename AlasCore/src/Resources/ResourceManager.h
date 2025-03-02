#pragma once

#include <unordered_map>
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"

namespace Alas
{
    class ResourceManager
    {
    public:
        static void AddResourceToRegistry(UID id, const std::string& filepath);
        static const std::string& GetResourceFilepath(UID id);
        static UID GetResourceIdByPath(const std::string& filepath);
        
        static void AddUsedResource(UID id, const Shared<Shader>& shader);
        static void AddUsedResource(UID id, const Shared<Texture>& texture);
        
        static const Shared<Shader>& GetUsedShader(UID id);
        static const Shared<Texture>& GetUsedTexture(UID id);

    private:
        static std::unordered_map<UID, std::string> ResourcesRegistry;

        static std::unordered_map<UID, Shared<Shader>> UsedShaders;
        static std::unordered_map<UID, Shared<Texture>> UsedTextures;
    };   
} // namespace Alas
