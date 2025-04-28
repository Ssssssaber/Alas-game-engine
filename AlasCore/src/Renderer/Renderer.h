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
        static void Submit2D(const Shared<Texture>& texture, const Shared<Shader>& shader, const glm::vec4& color, const glm::mat4& modelMatrix, UID uid = 0);

        static void DrawLine(const glm::vec3& position, const float rotation, const glm::vec3& scale);
        static void DrawBox(const glm::vec3& position, const float rotation, const glm::vec3& scale);
        
        static void SubmitOverlayText(const std::string& text, const glm::vec2& position, float rotation, const glm::vec2& scale, glm::vec4 color = glm::vec4(1.0f));
        static void SubmitWorldSpaceText(const std::string& text, const glm::vec3& position, float rotation, const glm::vec2& scale, glm::vec4 color);

        static void Submit(const Shared<VertexArray>& vertexArray, const Shared<Shader>& shader, const glm::vec4& color,  const glm::mat4& modelMatrix);
        static void Submit(Entity& entity);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

    private:
		static RendererAPI::API _rendererAPI;
        struct RendererData 
        {            
            Shared<OrthCamera> Camera;

            Shared<VertexArray> lineVertexArray;
            Shared<VertexArray> lineQuadVertexArray;
            Shared<Shader> lineShader;

            Shared<VertexArray> QuadVertexArray;

            Shared<Shader> OverlayTextShader;
            Shared<Shader> WorldSpaceTextShader;
            Shared<TextRendering> TextRenderer;
        };

        static RendererData _Data;
	};
}