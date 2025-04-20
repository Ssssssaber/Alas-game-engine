#pragma once

#include "Renderer/OrthCamera.h"
#include "Entity/Entity.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/RendererAPI.h"
#include "Renderer/TextRendering.h"
#include "glm.hpp"
namespace Alas {
	class Renderer
	{
	public:
        static void Init();
        static void BeginScene(const Shared<OrthCamera>& camera);
        static void EndScene();

        static void DrawQuad(const Shared<Shader>& shader, const glm::vec4& color, const glm::mat4& modelMatrix);
        static void Submit2D(const Shared<Texture>& texture, const Shared<Shader>& shader, const glm::vec4& color, const glm::mat4& modelMatrix);
        static void SubmitText(const std::string& text, glm::vec3 position, float rotation, float scale, glm::vec4 color = glm::vec4(1.0f));
        static void Submit(const Shared<VertexArray>& vertexArray, const Shared<Shader>& shader, const glm::vec4& color,  const glm::mat4& modelMatrix);
        static void Submit(Entity& entity);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

    private:
		static RendererAPI::API _rendererAPI;
        struct RendererData 
        {
            Shared<OrthCamera> Camera;
            Shared<VertexArray> QuadVertexArray;
            Shared<Shader> TextShader;
            Shared<TextRendering> TextRenderer;
        };

        static RendererData _Data;
	};
}