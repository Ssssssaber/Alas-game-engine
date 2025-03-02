#include <yaml-cpp/yaml.h>
#include "Scene.h"
#include "Entity.h"
#include "Resources/ResourceManager.h"

namespace Alas
{
    
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

    std::string RigidBody2DTypeToString(RigidBody2D::BodyType type)
    {
        switch (type)
        {
            case (RigidBody2D::BodyType::Dynamic): return "Dynamic";            
            case (RigidBody2D::BodyType::Kinematic): return "Kinematic";
            case (RigidBody2D::BodyType::Static): return "Static";
        }
        return "None";
    } 

    class SceneSerialization
    {
    public:
        // SceneSerialization(/* args */);
        // ~SceneSerialization();

        static void SerializeScene(const Shared<Scene>& scene, const std::string& filepath)
        {
            // YAML::Node node;
            YAML::Emitter out;
            out << YAML::BeginMap;
            out << YAML::Key << "Scene Name" << YAML::Value << scene->Name;
            
            out << YAML::Key << "Entities";
            out << YAML::Key << YAML::BeginMap;

            for (auto it = scene->GetEntityMap().begin(); it != scene->GetEntityMap().end(); it++)
            {
                Entity entity = it->second;
                out << YAML::Key << it->first;
                
                out << YAML::BeginMap;

                if (entity.HasComponent<TagComponent>())
                {
                    auto& tag = entity.GetComponent<TagComponent>();
                    out << YAML::Key << "Tag" << YAML::Value << tag.Tag;
                }

                if (entity.HasComponent<Transform>())
                {
                    auto& transform = entity.GetComponent<Transform>();
                    out << YAML::Key << "Transform";
                    
                    out << YAML::BeginMap;

                    out << YAML::Key << "Position" << YAML::Value << transform.Position;
                    out << YAML::Key << "Rotation" << YAML::Value << transform.Rotation;
                    out << YAML::Key << "Scale" << YAML::Value << transform.Scale;

                    out << YAML::EndMap;
                }

                if (entity.HasComponent<SpriteComponent>())
                {
                    auto& sprite = entity.GetComponent<SpriteComponent>();
                    out << YAML::Key << "Sprite";
                    
                    out << YAML::BeginMap;

                    out << YAML::Key << "Shader" << YAML::Value << ResourceManager::GetResourceFilepath(sprite.c_Shader->GetUID());
                    out << YAML::Key << "Texture" << YAML::Value << ResourceManager::GetResourceFilepath(sprite.c_Texture->GetUID());
                    out << YAML::Key << "Color" << YAML::Value << sprite.Color;

                    out << YAML::EndMap;
                }

                if (entity.HasComponent<RigidBody2D>())
                {
                    auto& rigid = entity.GetComponent<RigidBody2D>();
                    out << YAML::Key << "RigidBody2D";
                    
                    out << YAML::BeginMap;

                    out << YAML::Key << "Type" << YAML::Value << RigidBody2DTypeToString(rigid.Type);
                    out << YAML::Key << "Mass" << YAML::Value << rigid.Mass;
                    out << YAML::Key << "Gravity scale" << YAML::Value << rigid.GravityScale;

                    out << YAML::EndMap;
                }

                if (entity.HasComponent<BoxCollider2D>())
                {
                    auto& collider = entity.GetComponent<BoxCollider2D>();
                    out << YAML::Key << "BoxCollider2D";
                    
                    out << YAML::BeginMap;

                    out << YAML::Key << "Offset" << YAML::Value << collider.Offset;
                    out << YAML::Key << "Size" << YAML::Value << collider.Size;

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
    };
} // namespace Alas
