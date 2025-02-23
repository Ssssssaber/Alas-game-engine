#pragma once

#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Texture2D.h"


#include <glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/quaternion.hpp>
#include <string>

namespace Alas
{
    struct IDComponent
	{
		UID ID;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
	};

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
	};

	struct Transform
	{
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };
        
        glm::mat4 _modelMatrix;

		Transform() = default;
		Transform(const Transform&) = default;
		Transform(const glm::vec3& translation)
			: Position(translation) {}

        glm::mat4 CalculateModelMatrix()
        {
            return glm::translate(glm::mat4(1.0f), Position) *
                glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) *
                glm::scale(glm::mat4(1.0f), Scale);
        }
	};

    // Forward declaration
	class ScriptableEntity;

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity*(*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};

    struct SpriteComponent
    {
        Shared<Shader> Shader;
        Shared<Texture2D> Texture;
        glm::vec3 Color = glm::vec3(1.0f, 1.0f, 1.0f);

        SpriteComponent() = default;
		SpriteComponent(const SpriteComponent&) = default;
        SpriteComponent(Shared<Texture2D> texture, Shared<Alas::Shader> shader) :
            Texture(texture), Shader(shader) {}
    };

    struct RigidBody2D
    {
        enum class BodyType { Static, Kinematic, Dynamic};
        BodyType Type = BodyType::Static;
        double Mass = 1.0;
        glm::vec2 Velocity = glm::vec2(0.0f);
        glm::vec2 Force = glm::vec2(0.0f);
        bool AffectedByGravity = true;


        RigidBody2D() = default;
		RigidBody2D(const RigidBody2D&) = default;
        RigidBody2D(RigidBody2D::BodyType bodyType) :
            Type(bodyType) {}
        
    };

    struct BoxCollider2D
    {
        glm::vec2 Offset = {0.0f, 0.0f};
        glm::vec2 Size = {.5f, .5f};
        
        BoxCollider2D() = default;
		BoxCollider2D(const BoxCollider2D&) = default;
        BoxCollider2D(glm::vec2 offset, glm::vec2 size) :
            Offset(offset), Size(size) {}
        
    };
} // namespace Alas
