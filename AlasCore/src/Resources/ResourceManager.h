#pragma once

#include <unordered_map>

namespace Alas
{
    class ResourceManager
    {
    public:
        static void AddResource(UID id, const std::string& filepath);
        static const std::string& GetResourceFilepath(UID id);

    private:
        static std::unordered_map<UID, std::string> ResourcesFilapaths;
    };   
} // namespace Alas
