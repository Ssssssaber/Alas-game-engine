#include "Renderer.h"
#include "RendererCommand.h"
#include "Entity/Components.h"
namespace Alas
{
    Renderer::RendererData Renderer::_Data = RendererData();

    void Renderer::Init()
    {
        ALAS_PROFILE_FUNCTION()
        RenderCommand::EnableBlending();

        _Data.quadVertexArray.reset(VertexArray::Create());
        
        float quadVertices[5 * 4] = {
            -0.2f, -0.2f, 0.0f, 0.0f, 0.0f,
			 0.2f, -0.2f, 0.0f, 1.0f, 0.0f,
			 0.2f,  0.2f, 0.0f, 1.0f, 1.0f,
			-0.2f,  0.2f, 0.0f, 0.0f, 1.0f
        };
        
        Alas::Shared<Alas::VertexBuffer> quadVertexBuffer;
        quadVertexBuffer.reset(Alas::VertexBuffer::Create(quadVertices, sizeof(quadVertices)));
        
        {
            Alas::BufferLayout layout {
                {Alas::ShaderElementType::Float3, "a_Position"},
                {Alas::ShaderElementType::Float2, "a_TexCoord"}
            };
            quadVertexBuffer->SetLayout(layout);
        }
    
        _Data.quadVertexArray->AddVertexBuffer(quadVertexBuffer);
        
        uint32_t quadIndices[6] = { 0, 1, 2, 2, 3, 0 };        
        Alas::Shared<Alas::IndexBuffer> quadIndexBuffer;
        quadIndexBuffer.reset(Alas::IndexBuffer::Create(quadIndices, sizeof(quadIndices) / sizeof(uint32_t)));
        
        _Data.quadVertexArray->SetIndexBuffer(quadIndexBuffer);
    }

    void Renderer::BeginScene(const Shared<OrthCamera>& camera)
	{
        ALAS_PROFILE_FUNCTION();
        _Data.ViewProjectionMatrix = camera->GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
        ALAS_PROFILE_FUNCTION();
	}

    void Renderer::Submit2D(const Shared<Texture>& texture, const Shared<Shader>& shader, const glm::vec4& color, const glm::mat4& modelMatrix)
    {
        ALAS_PROFILE_FUNCTION()
        texture->Bind();
        shader->Bind();
        shader->setMat4("u_viewProjectionMatrix", _Data.ViewProjectionMatrix);
        shader->setMat4("u_model", modelMatrix);
        shader->setVec4("u_Color", color.x, color.y, color.z, color.w);
        shader->setInt("u_Texture", 0);
        
		RenderCommand::DrawIndexed(_Data.quadVertexArray);
    }

} // namespace AGS
