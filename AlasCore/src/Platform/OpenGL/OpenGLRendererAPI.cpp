#include "OpenGLRendererAPI.h"
#include "OpenGLCore.h"

namespace Alas {

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		GlCall(glClearColor(color.r, color.g, color.b, color.a));
	}

	void OpenGLRendererAPI::Clear()
	{
		GlCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
}