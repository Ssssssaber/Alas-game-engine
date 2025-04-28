#include "OpenGLRendererAPI.h"
#include "OpenGLCore.h"

namespace Alas {

    void OpenGLRendererAPI::EnableBlending()
    {
        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glLineWidth(2);
    }

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		GlCall(glClearColor(color.r, color.g, color.b, color.a));
	}

	void OpenGLRendererAPI::Clear()
	{
		GlCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	void OpenGLRendererAPI::DrawIndexed(const Shared<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		GlCall(glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr));
	}

	void OpenGLRendererAPI::DrawLine(const Shared<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		GlCall(glDrawArrays(GL_LINE_LOOP, 0, vertexArray->GetIndexBuffer()->GetCount()));
	}
}