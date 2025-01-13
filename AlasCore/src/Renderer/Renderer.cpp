#include "Renderer.h"
#include "RendererCommand.h"
namespace Alas
{
    Renderer::SceneData Renderer::_Data = SceneData();

    void Renderer::BeginScene(const Shared<OrthCamera>& camera)
	{
        _Data.ViewProjectionMatrix = camera->GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const Shared<VertexArray>& vertexArray, 
        const Shared<Shader>& shader, const glm::mat4& modelMatrix = glm::mat4(1.0f))
	{
        shader->Bind();
        shader->setMat4("u_viewProjectionMatrix", _Data.ViewProjectionMatrix);
        shader->setMat4("u_model", modelMatrix);
        

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

    void Renderer::Submit(const Shared<GameObject>& gameObject)
    {
        Shared<Shader> shader = gameObject->GetShader();
        
        shader->setMat4("u_viewProjectionMatrix", _Data.ViewProjectionMatrix);
        shader->setMat4("u_model", gameObject->GetModelMatrix());
        shader->setVec3("u_color", gameObject->GetColor());

		Shared<VertexArray> vertexArray = gameObject->GetVertexArray();
        vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
} // namespace AGS
