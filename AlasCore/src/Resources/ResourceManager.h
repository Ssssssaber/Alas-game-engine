#pragma once

#include <unordered_map>
#include <filesystem>

#include "Renderer/Shader.h"
#include "Renderer/Texture.h"

namespace fs = std::filesystem;

// #define ASSETS_DIR fs::current_pat().append("Assets")
#define ASSETS_DIR "Assets"
namespace Alas
{
    class ResourceManager
    {
    public:
        // 
        // static void UpdateResourceFilesRegistry()
        // 1. recursive loop through assets directory. For each directory:
        //     * for each file that does not have meta file - create it (how to differ assets?)
        //     * add filepaths to registry with id taken from meta file
        //     * check remaining meta files in a directory - if no resource file then delete meta file
        // ------------------------------------------
        // differing assets by their file extentions:
        // * shaders: .shader
        // * scripting: .lua
        // * textures: .png
        // * add later
        //      fonts: .ttf
        //      sound: .mp3
        static void Init();
        static Shared<Shader> GetBaseShader() { return baseShader; }
        static Shared<Texture> GetBaseTexture() { return baseTexture; }

        static UID GetResourceIdByFilepath(const std::string& filepath);
        static UID GetResourceIdByFilepath(const fs::path& filepath);
        
        static void UpdateMetaFiles() { UpdateMetaFiles(ASSETS_DIR); }
        static void UpdateMetaFiles(const std::string& directory);
        // runtime logic
        static void AddResourceToRegistry(UID id, const std::string& filepath);
        static std::string GetResourceFilepathString(UID id);

        static std::vector<std::string> GetFilesWithExtension(const std::string& extention);
        
        static void AddUsedResource(UID id, const Shared<Shader>& shader);
        static void AddUsedResource(UID id, const Shared<Texture>& texture);
        
        static bool IsShaderUsed(UID uid);
        static bool IsTextureUsed(UID uid);

        static Shared<Shader> GetShader(UID id);
        static Shared<Shader> ResourceManager::GetShader(const std::string& filepath);
        static Shared<Texture> GetTexture(UID id);
        static Shared<Texture> ResourceManager::GetTexture(const std::string& filepath);

    private:
        static void CheckRecourcesPathIntegrity();
    private:
        static std::unordered_map<UID, fs::path> ResourceFilesRegistry;

        static std::unordered_map<UID, Shared<Shader>> UsedShaders;
        static std::unordered_map<UID, Shared<Texture>> UsedTextures;

        static Shared<Shader> baseShader;
        static Shared<Texture> baseTexture;
    };   
} // namespace Alas
