#include "ResourceManager.h"
#include <yaml-cpp/yaml.h>

#include <fstream>



#define ASSET_ID "asset_uid"
namespace Alas
{
    std::unordered_map<UID, fs::path> ResourceManager::ResourceFilesRegistry;  
    std::unordered_map<UID, Shared<Shader>> ResourceManager::UsedShaders;
    std::unordered_map<UID, Shared<Texture>> ResourceManager::UsedTextures;

    Shared<Shader> ResourceManager::baseShader;
    Shared<Texture> ResourceManager::baseTexture;

    void ResourceManager::Init()
    {
        std::string vertexSrc = R"(
        #version 450
                
        layout(location = 0) in vec3 a_Position;
        layout(location = 1) in vec2 a_TexCoord;

        out vec2 o_TexCoord;
        out flat int o_EntityId;

        uniform mat4 u_viewProjectionMatrix;
        uniform mat4 u_model;
        uniform int u_EntityId;

        void main()
        {
            gl_Position = u_viewProjectionMatrix * u_model * vec4(a_Position.xy, 0.0, 1.0);	
            
            o_TexCoord = a_TexCoord;
            o_EntityId = u_EntityId;
        })";

        std::string fragmentSrc = R"(
        #version 450

        out vec4 color;
        out int color2;

        in vec2 o_TexCoord;
        in flat int o_EntityId;

        uniform sampler2D u_Texture;
        uniform vec4 u_Color;

        void main()
        {
            color = texture(u_Texture, o_TexCoord) * u_Color;
            color2 = o_EntityId;
        }
        )";

        baseShader = Shader::Create(GetUniqueId(), vertexSrc, fragmentSrc);

        baseTexture = Texture::Create(GetUniqueId(), 1, 1);
        uint32_t whiteTextureData = 0xffffffff;
		baseTexture->SetData(&whiteTextureData, sizeof(uint32_t));
    }

    UID ResourceManager::GetResourceIdByFilepath(const std::string& filepath)
    {
        return GetResourceIdByFilepath(fs::path(filepath));
    }
    
    UID ResourceManager::GetResourceIdByFilepath(const fs::path& filepath)
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
    
                fs::path meta_file_path = full_path.string() + ".meta";
                if (fs::exists(meta_file_path))
                {
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

    std::vector<std::string> ResourceManager::GetFilesWithExtension(const std::string& extention)
    {
        std::vector<std::string> files;
        for (auto it = ResourceFilesRegistry.begin(); it != ResourceFilesRegistry.end(); it++)
        {
            if (it->second.extension() == extention) files.push_back(it->second.string());
        }

        return files;
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
                return NULL_STRING;    
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

    bool ResourceManager::IsShaderUsed(UID uid)
    {
        if (UsedShaders.find(uid) != UsedShaders.end())
        {
            return true;
        }

        return false;
    }

    bool ResourceManager::IsTextureUsed(UID uid)
    {
        if (UsedTextures.find(uid) != UsedTextures.end())
        {
            return true;
        }
        return false;
    }

    Shared<Shader> ResourceManager::GetShader(const std::string& filepath)
    {
        return GetShader(GetResourceIdByFilepath(filepath));
    }

    Shared<Shader> ResourceManager::GetShader(UID id)
    {
        auto findResult = UsedShaders.find(id);
        if (findResult == UsedShaders.end())
        {
            std::string filepath = GetResourceFilepathString(id);
            if (filepath != NULL_STRING)
            {
                Shared<Shader> shader = Shader::Create(filepath, id);
                if (shader)
                {
                    UsedShaders[id] = shader;
                    return shader;
                }
            }
            ALAS_CORE_WARN(
                "Resource with id {0} does not exist.", id);
            return baseShader;
        }
        return findResult->second;
    }

    Shared<Texture> ResourceManager::GetTexture(const std::string& filepath)
    {
        return GetTexture(GetResourceIdByFilepath(filepath));
    }
    
    Shared<Texture> ResourceManager::GetTexture(UID id)
    {
        auto findResult = UsedTextures.find(id);
        if (findResult == UsedTextures.end())
        {
            std::string filepath = GetResourceFilepathString(id);
            if (filepath != NULL_STRING)
            {
                Shared<Texture> texture = Texture::Create(filepath, id);
                if (texture)
                {
                    UsedTextures[id] = texture;
                    return texture;
                }
            }
            ALAS_CORE_WARN(
                "Resource with id {0} does not exist.", id);
            return baseTexture;
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
