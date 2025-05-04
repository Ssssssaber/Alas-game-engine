#include "ResourceManager.h"
#include <yaml-cpp/yaml.h>

#include <fstream>

#define ASSET_ID "asset_uid"
namespace Alas
{
    std::unordered_map<UID, fs::path> ResourceManager::ResourceFilesRegistry;  
    std::unordered_map<UID, Shared<Shader>> ResourceManager::UsedShaders;
    std::unordered_map<UID, Shared<Texture>> ResourceManager::UsedTextures;

    UID ResourceManager::RecourceExists(const std::string& filepath)
    {
        return RecourceExists(fs::path(filepath));
    }
    
    UID ResourceManager::RecourceExists(const fs::path& filepath)
    {
        if (!fs::exists(filepath))
        {
            ALAS_CORE_ERROR("Recource {0} does not exist", filepath.string());
            return NULL_UID;
        }
        for (auto it = ResourceFilesRegistry.begin(); it != ResourceFilesRegistry.end(); ++it) 
        {
            if (!fs::equivalent(it->second, filepath)) continue;
            return it->first;
        }

        return NULL_UID;
    }

    void ResourceManager::UpdateMetaFiles(const std::string& path)
    {
        // Clearing is the most stable way but in future this need to be replaced with:
        // 1. Checking for file integrity - CheckRecourcesPathIntegrity(). Plus it probably lacks .meta files check.
        // 2. Skipping filepaths that are already in the registry since they were already checked
        // ... could be even more checks to guarantee safety of all recources
        ResourceFilesRegistry.clear();

        for (const auto& p : fs::recursive_directory_iterator(fs::path(path))) {
            if (!fs::is_directory(p)) {
                // Get the full path
                fs::path full_path = p.path();
                
                // Get the file extension
                std::string extension = full_path.extension().string();
                // Get the filename (without the extension)
                std::string filename = full_path.stem().string();
    
                // check if resource that .meta file is linked to exists
                if (extension == ".meta") 
                {
                    fs::path originalFile = full_path.parent_path().append(filename);
                    if (!fs::exists(originalFile)) fs::remove(full_path);
                    continue;
                }
    
                // Output the full path, filename, and extension
                std::cout << "Full path: " << full_path << '\n';
                std::cout << "Filename: " << filename << '\n';
                std::cout << "Extension: " << extension << '\n';
    
                fs::path meta_file_path = full_path.string() + ".meta";
                if (fs::exists(meta_file_path))
                {
                    std::cout << ".meta file : " << meta_file_path << '\n';

                    // read meta file and get uid
                    std::string stringMetaFilePath = meta_file_path.string();
                    YAML::Node metaNode;

                    try
                    {
                        metaNode = YAML::LoadFile(meta_file_path.string());
                        if (!metaNode[ASSET_ID])  continue;
                        UID uid = metaNode[ASSET_ID].as<UID>();

                        // add uid and filepath to registry
                        ResourceFilesRegistry[uid] = full_path.string();
                    }
                    catch (YAML::Exception e)
                    {
                        ALAS_CORE_ERROR("Meta file corrupted: '{0}'\n     {1}", stringMetaFilePath, e.what());
                        continue;
                    }
                }
                else
                {
                    // genereate new id
                    UID newUID = GetUniqueId();
                    
                    // add resource to registry
                    ResourceFilesRegistry[newUID] = full_path.string();

                    // create meta file
                    std::ofstream ofs(meta_file_path);
                    YAML::Emitter out;
                    out << YAML::BeginMap;
                    out << YAML::Key << ASSET_ID << YAML::Value << newUID;
                    out << YAML::EndMap;

                    // write meta file
                    std::ofstream fout(meta_file_path);
                    fout << out.c_str();
                    assert(out.good());
                }
                    
            }
        }
    }

    UID ResourceManager::GetResourceIdByPath(const std::string& filepath)
    {
        for (auto it = ResourceFilesRegistry.begin(); it != ResourceFilesRegistry.end(); ++it) 
        {
            if (it->second == filepath) return it->first;
        }

        return 0;
    }

    std::string ResourceManager::GetResourceFilepathString(UID id)
    {
        if (ResourceFilesRegistry.find(id) == ResourceFilesRegistry.end())
        {
            ALAS_CORE_WARN(
                "Resource with id {0} does not exit. Refreshing registry ...", id);

            UpdateMetaFiles();
            if (ResourceFilesRegistry.find(id) == ResourceFilesRegistry.end())
            {
                ALAS_ASSERT(false, 
                    "Resource with id {0} was not found ...", id);        
            }
        }
        return ResourceFilesRegistry[id].string();
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

    Shared<Shader> ResourceManager::IsShaderUsed(UID id)
    {
        auto findResult = UsedShaders.find(id);
        if (findResult == UsedShaders.end())
        {
            ALAS_CORE_WARN(
                "Resource with id {0} does not exist.", id);
            return nullptr;
        }
        return findResult->second;
    }
    
    Shared<Texture> ResourceManager::IsTextureUsed(UID id)
    {
        auto findResult = UsedTextures.find(id);
        if (findResult == UsedTextures.end())
        {
            ALAS_CORE_WARN(
                "Resource with id {0} does not exist.", id);
            return nullptr;
        }
        return findResult->second;
    }

    void ResourceManager::CheckRecourcesPathIntegrity()
    {
        if (ResourceFilesRegistry.empty()) return;

        for (auto it = ResourceFilesRegistry.begin(); it != ResourceFilesRegistry.end(); it++)
        {
            if (fs::exists(it->second)) continue;

            ResourceFilesRegistry.erase(it->first);
        }
    }

} // namespace Alas
