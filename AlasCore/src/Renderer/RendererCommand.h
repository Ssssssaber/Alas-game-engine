#pragma once

#include "RendererAPI.h"

namespace Alas {

	class RenderCommand
	{
	public:
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