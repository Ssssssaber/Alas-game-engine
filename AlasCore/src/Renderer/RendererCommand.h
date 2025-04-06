#pragma once

#include "RendererAPI.h"
#include "Core/Window.h"
namespace Alas {

	class RenderCommand
	{
	public:
        inline static void EnableBlending()
        {
			ALAS_PROFILE_FUNCTION();
            _rendererAPI->EnableBlending();
        }
        inline static void SetActiveWindow(const Window& window)
        {
			ALAS_PROFILE_FUNCTION();
            window.SetAsCurrent();
        }
		inline static void SetClearColor(const glm::vec4& color)
		{
			ALAS_PROFILE_FUNCTION();
            _rendererAPI->SetClearColor(color);
		}
		inline static void Clear()
		{
			ALAS_PROFILE_FUNCTION();
			_rendererAPI->Clear();
		}
		inline static void DrawIndexed(const Shared<VertexArray>& vertexArray)
		{
			ALAS_PROFILE_FUNCTION();
			_rendererAPI->DrawIndexed(vertexArray);
		}
		static void DrawLine(const Shared<VertexArray>& vertexArray)
		{
			ALAS_PROFILE_FUNCTION();
			_rendererAPI->DrawLine(vertexArray);
		}
	private:
		static RendererAPI* _rendererAPI;
	};
}