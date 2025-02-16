#pragma once

#include "Renderer/RendererAPI.h"

namespace Alas {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
        virtual void EnableBlending() override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;
		virtual void DrawIndexed(const Shared<VertexArray>& vertexArray) override;
	};

}