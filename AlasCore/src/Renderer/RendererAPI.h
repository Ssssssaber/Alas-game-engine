#pragma once

#include <glm.hpp>
#include "VertexArray.h"

namespace Alas {
	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};
	public:
        virtual void EnableBlending() {}
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;
		virtual void DrawIndexed(const Shared<VertexArray>& vertexArray) = 0;
		virtual void DrawLine(const Shared<VertexArray>& vertexArray) = 0;
		inline static API GetAPI() { return _API; }
	private:
		static API _API;
	};
}