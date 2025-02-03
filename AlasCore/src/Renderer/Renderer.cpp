#include "Renderer.h"
#include "RendererCommand.h"
#include "Entity/Components.h"
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
        const Shared<Shader>& shader, const glm::vec3& color, const glm::mat4& modelMatrix = glm::mat4(1.0f))
	{
        shader->Bind();
        shader->setMat4("u_viewProjectionMatrix", _Data.ViewProjectionMatrix);
        shader->setMat4("u_model", modelMatrix);
        shader->setVec4("u_Color", color.x, color.y, color.z, 1.0f);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

    void Renderer::Submit(Entity& entity)
    {
        auto& mesh = entity.GetComponent<MeshComponent>();
        auto& transform = entity.GetComponent<TransformComponent>();
        
        mesh.Shader->Bind();
        mesh.Shader->setMat4("u_viewProjectionMatrix", _Data.ViewProjectionMatrix);
        mesh.Shader->setMat4("u_model", transform.CalculateModelMatrix());
        mesh.Shader->setVec3("u_color", mesh.Color);
        
        mesh.Shader->setVec4("u_Color", mesh.Color.x, mesh.Color.y, mesh.Color.z, 1.0f);

        mesh.VertexArray->Bind();
		RenderCommand::DrawIndexed(mesh.VertexArray);
	}
} // namespace AGS
