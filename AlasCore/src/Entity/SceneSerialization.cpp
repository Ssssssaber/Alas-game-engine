#include "SceneSerialization.h"

#include <yaml-cpp/yaml.h>
#include "Resources/ResourceManager.h"
#include "Entity.h"
#include "Entity/Components.h"

// keys for yaml file
#define SCENE_NAME "Scene name"
#define ENTITIES "Entities"

namespace YAML {

	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

namespace Alas
{
    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& vec)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << vec.x << vec.y << vec.z << vec.w << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& vec)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << vec.x << vec.y << vec.z << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& vec)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << vec.x << vec.y << YAML::EndSeq;
        return out;
    }

    bool CheckKeyExists(YAML::Node& node, const std::string& key, const std::string& filepath)
    {
        if (!node[key]) 
        {
            ALAS_ASSERT(false, filepath + " file does not have " + key + " key!");
            return false;
        }

        return true;
    }

    void SceneSerialization::SerializeScene(const Shared<Scene>& scene, const std::string& filepath)
    {
        ALAS_PROFILE_FUNCTION();
        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << SCENE_NAME << YAML::Value << scene->Name;
        
        out << YAML::Key << ENTITIES;
        out << YAML::Key << YAML::BeginMap;

        for (auto it = scene->GetEntityMap().begin(); it != scene->GetEntityMap().end(); it++)
        {
            Entity entity = it->second;
            out << YAML::Key << it->first;
            
            out << YAML::BeginMap;

            if (entity.HasComponent<TagComponent>())
            {
                auto& tag = entity.GetComponent<TagComponent>();
                out << YAML::Key << TAG_C << YAML::Value << tag.Tag;
            }

            if (entity.HasComponent<Transform>())
            {
                auto& transform = entity.GetComponent<Transform>();
                out << YAML::Key << TRANSFORM_C;
                
                out << YAML::BeginMap;

                out << YAML::Key << TRANSFORM_C_POSITION << YAML::Value << transform.Position;
                out << YAML::Key << TRANSFORM_C_ROTATION << YAML::Value << transform.Rotation;
                out << YAML::Key << TRANSFORM_C_SCLAE << YAML::Value << transform.Scale;

                out << YAML::EndMap;
            }

            if (entity.HasComponent<SpriteComponent>())
            {
                auto& sprite = entity.GetComponent<SpriteComponent>();
                out << YAML::Key << SPRITE_C;
                
                out << YAML::BeginMap;

                out << YAML::Key << SPRITE_C_SHADER << YAML::Value << ResourceManager::GetResourceFilepath(sprite.c_Shader->GetUID());
                out << YAML::Key << SPRITE_C_TEXTURE << YAML::Value << ResourceManager::GetResourceFilepath(sprite.c_Texture->GetUID());
                out << YAML::Key << SPRITE_C_COLOR << YAML::Value << sprite.Color;

                out << YAML::EndMap;
            }

            if (entity.HasComponent<RigidBody2D>())
            {
                auto& rigid = entity.GetComponent<RigidBody2D>();
                out << YAML::Key << RIGID_BODY_2D_C;
                
                out << YAML::BeginMap;

                out << YAML::Key << RIGID_BODY_2D_C_TYPE << YAML::Value << RigidBody2D::TypeToString(rigid.Type);
                out << YAML::Key << RIGID_BODY_2D_C_MASS << YAML::Value << rigid.Mass;
                out << YAML::Key << RIGID_BODY_2D_C_GRAVITY_SCALE << YAML::Value << rigid.GravityScale;

                out << YAML::EndMap;
            }

            if (entity.HasComponent<BoxCollider2D>())
            {
                auto& collider = entity.GetComponent<BoxCollider2D>();
                out << YAML::Key << BOX_COLLIDER_2D_C;
                
                out << YAML::BeginMap;

                out << YAML::Key << BOX_COLLIDER_2D_C_OFFSET << YAML::Value << collider.Offset;
                out << YAML::Key << BOX_COLLIDER_2D_C_SIZE << YAML::Value << collider.Size;

                out << YAML::EndMap;
            }

            out << YAML::EndMap;
        }

        out << YAML::EndMap;
        out << YAML::EndMap;

        std::ofstream fout(filepath);
        fout << out.c_str();
        assert(out.good());
    }

    Shared<Scene> SceneSerialization::DeserializeScene(const std::string& filepath)
    {
        ALAS_PROFILE_FUNCTION();
        YAML::Node sceneNode = YAML::LoadFile(filepath);
        Shared<Scene> scene;
        scene.reset(new Scene());

		try
		{
			sceneNode = YAML::LoadFile(filepath);
		}
		catch (YAML::ParserException e)
		{
			ALAS_CORE_ERROR("Failed to load scene file '{0}'\n     {1}", filepath, e.what());
			return scene;
		}
        catch (YAML::BadFile e)
		{
			ALAS_CORE_ERROR("Failed to load scene file '{0}'\n     {1}", filepath, e.what());
			return scene;
		}

        if (!CheckKeyExists(sceneNode, SCENE_NAME, filepath) ||
            !CheckKeyExists(sceneNode, ENTITIES, filepath)) return scene;

        scene->Name = sceneNode[SCENE_NAME].as<std::string>();

        for (auto entityNode : sceneNode[ENTITIES])
        {
            UID id = entityNode.first.as<UID>();
            auto data = entityNode.second;
            if (!data[TAG_C])
            {
                ALAS_CORE_ERROR("Entity with id {0} has no tag component", id);   
            }

            Entity ent = scene->CreateEntityWithId(data[TAG_C].as<std::string>(), id);
            
            auto& transform = ent.GetComponent<Transform>();

            auto transformData = data[TRANSFORM_C];
            if (transformData)
            {        
                transform.Position = transformData[TRANSFORM_C_POSITION].as<glm::vec3>();
                transform.Rotation = transformData[TRANSFORM_C_ROTATION].as<glm::vec3>();
                transform.Scale = transformData[TRANSFORM_C_SCLAE].as<glm::vec3>();
            }

            auto spriteData = data[SPRITE_C];
            if (spriteData)
            {
                // implement sprite
                auto& sprite = ent.AddComponent<SpriteComponent>();

                {
                    const std::string& shaderFilepath = spriteData[SPRITE_C_SHADER].as<std::string>();
                    UID shaderID = ResourceManager::GetResourceIdByPath(shaderFilepath);
                    if (shaderID == 0)
                    {
                        sprite.c_Shader = Shader::Create(shaderFilepath);
                    }
                    else
                    {
                        sprite.c_Shader = ResourceManager::GetUsedShader(shaderID);
                    }
                }
                {
                    const std::string& textureFilepath = spriteData[SPRITE_C_TEXTURE].as<std::string>();
                    UID textureID = ResourceManager::GetResourceIdByPath(textureFilepath);
                    if (textureID == 0)
                    {
                        sprite.c_Texture = Texture::Create(textureFilepath);
                    }
                    else
                    {
                        sprite.c_Texture = ResourceManager::GetUsedTexture(textureID);
                    }
                    
                }
                
                sprite.Color = spriteData[SPRITE_C_COLOR].as<glm::vec4>();
            }
            
            auto rigidBody2DData = data[RIGID_BODY_2D_C];
            if (rigidBody2DData)
            {
                auto& rigidBody = ent.AddComponent<RigidBody2D>();
                rigidBody.Type = RigidBody2D::StringToType(rigidBody2DData[RIGID_BODY_2D_C_TYPE].as<std::string>());
                rigidBody.Mass = rigidBody2DData[RIGID_BODY_2D_C_MASS].as<float>();
                rigidBody.GravityScale = rigidBody2DData[RIGID_BODY_2D_C_GRAVITY_SCALE].as<float>();
            }
            
            auto boxCollider2DData = data[BOX_COLLIDER_2D_C];
            if (boxCollider2DData)
            {
                auto& boxCollider = ent.AddComponent<BoxCollider2D>();
                boxCollider.Offset = boxCollider2DData[BOX_COLLIDER_2D_C_OFFSET].as<glm::vec2>();
                boxCollider.Size = boxCollider2DData[BOX_COLLIDER_2D_C_SIZE].as<glm::vec2>();
            }
        }

        return scene;
    }
} // namespace Alas
