#include "Renderer.h"
#include "RendererCommand.h"
namespace Alas
{
    Renderer::SceneData Renderer::_Data = SceneData();

    void Renderer::BeginScene(OrthCamera* camera)
	{
        _Data.ViewProjectionMatrix = camera->GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader)
	{
        shader->setMat4("u_viewProjectionMatrix", _Data.ViewProjectionMatrix);
        shader->Bind();

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
} // namespace AGS
