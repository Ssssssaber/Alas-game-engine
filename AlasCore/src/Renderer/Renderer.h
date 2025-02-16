#pragma once

#include "Renderer/OrthCamera.h"
#include "Entity/Entity.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture2D.h"
#include "Renderer/RendererAPI.h"
#include "glm.hpp"
namespace Alas {
	class Renderer
	{
	public:
        static void Init();
        static void BeginScene(const Shared<OrthCamera>& camera);
        static void EndScene();

        static void DrawQuad(const Shared<Shader>& shader, const glm::vec3& color, const glm::mat4& modelMatrix);
        static void Submit2D(const Shared<Texture2D>& texture, const Shared<Shader>& shader, const glm::vec3& color, const glm::mat4& modelMatrix);

        static void Submit(const Shared<VertexArray>& vertexArray, const Shared<Shader>& shader, const glm::vec3& color,  const glm::mat4& modelMatrix);
        static void Submit(Entity& entity);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

    private:
		static RendererAPI::API _rendererAPI;
        struct RendererData 
        {
            glm::mat4 ViewProjectionMatrix;
            Shared<VertexArray> quadVertexArray;
        };

        static RendererData _Data;
	};
}