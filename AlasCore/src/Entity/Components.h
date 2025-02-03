#pragma once

#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"

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

	struct TransformComponent
	{
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };
        
        glm::mat4 _modelMatrix;

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
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

    struct MeshComponent
    {
        Shared<Shader> Shader;
        Shared<VertexArray> VertexArray;
        glm::vec3 Color = glm::vec3(0.5f, 0.5f, 0.5f);

        MeshComponent() = default;
		MeshComponent(const MeshComponent&) = default;
        MeshComponent(Shared<Alas::Shader> shader, Shared<Alas::VertexArray> array) :
            Shader(shader), VertexArray(array) {}
    };
} // namespace Alas
