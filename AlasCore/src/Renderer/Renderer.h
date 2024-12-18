#pragma once

#include "Core/OrthCamera.h"
#include "Renderer/Shader.h"
#include "Renderer/RendererAPI.h"
#include "glm.hpp"
namespace Alas {
	class Renderer
	{
	public:
        static void BeginScene(OrthCamera* camera);
        static void EndScene();

        static void Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader);

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