#pragma once

#include "RendererAPI.h"
#include "Core/Window.h"
namespace Alas {

	class RenderCommand
	{
	public:
        inline static void SetActiveWindow(const Window& window)
        {
            window.SetAsCurrent();
        }
		inline static void SetClearColor(const glm::vec4& color)
		{
            _rendererAPI->SetClearColor(color);
		}
		inline static void Clear()
		{
			_rendererAPI->Clear();
		}
		inline static void DrawIndexed(const Shared<VertexArray>& vertexArray)
		{
			_rendererAPI->DrawIndexed(vertexArray);
		}
	private:
		static RendererAPI* _rendererAPI;
	};
}