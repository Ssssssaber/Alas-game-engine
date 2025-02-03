#pragma once

#include "Renderer/OrthCamera.h"
#include "Entity/Entity.h"
#include "Renderer/Shader.h"
#include "Renderer/RendererAPI.h"
#include "glm.hpp"
namespace Alas {
	class Renderer
	{
	public:
        static void BeginScene(const Shared<OrthCamera>& camera);
        static void EndScene();

        static void Submit(const Shared<VertexArray>& vertexArray, const Shared<Shader>& shader, const glm::vec3& color,  const glm::mat4& modelMatrix);
        static void Submit(Entity& entity);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

    private:
		static RendererAPI::API _rendererAPI;
        struct SceneData 
        {
            glm::mat4 ViewProjectionMatrix;
        };

        static SceneData _Data;
	};
}