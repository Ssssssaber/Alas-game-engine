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
        
        {
            // line part
            _Data.lineVertexArray.reset(VertexArray::Create());
            
            float lineVertices[] = {
                0.0f, 0.0f,
                0.0f, 0.5f
            };

            
            Alas::Shared<Alas::VertexBuffer> lineVertexBuffer;
            lineVertexBuffer.reset(Alas::VertexBuffer::Create(lineVertices, sizeof(lineVertices)));
            
            {
                Alas::BufferLayout layout {
                    {Alas::ShaderElementType::Float2, "a_Position"}
                };
                lineVertexBuffer->SetLayout(layout);
            }
        
            _Data.lineVertexArray->AddVertexBuffer(lineVertexBuffer);
            
            uint32_t lineIndices[2] = { 0, 1 };        
            Alas::Shared<Alas::IndexBuffer> lineIndexBuffer;
            lineIndexBuffer.reset(Alas::IndexBuffer::Create(lineIndices, sizeof(lineIndices) / sizeof(uint32_t)));
            
            _Data.lineVertexArray->SetIndexBuffer(lineIndexBuffer);

            _Data.lineShader = Shader::Create("Assets/Shaders/Line.shader");
        }

        {
            _Data.lineQuadVertexArray.reset(VertexArray::Create());
            
            float lineQuadVertices[2 * 4] = {
                -0.2f, -0.2f, 
                0.2f, -0.2f, 
                0.2f,  0.2f, 
                -0.2f,  0.2f, 
            };
            
            Alas::Shared<Alas::VertexBuffer> lineQuadVertexBuffer;
            lineQuadVertexBuffer.reset(Alas::VertexBuffer::Create(lineQuadVertices, sizeof(lineQuadVertices)));
            
            {
                Alas::BufferLayout layout {
                    {Alas::ShaderElementType::Float2, "a_Position"}
                };
                lineQuadVertexBuffer->SetLayout(layout);
            }
        
            _Data.lineQuadVertexArray->AddVertexBuffer(lineQuadVertexBuffer);
            
            uint32_t lineQuadIndices[4] = { 0, 1, 2, 3 };        
            Alas::Shared<Alas::IndexBuffer> lineQuadIndexBuffer;
            lineQuadIndexBuffer.reset(Alas::IndexBuffer::Create(lineQuadIndices, sizeof(lineQuadIndices) / sizeof(uint32_t)));
            
            _Data.lineQuadVertexArray->SetIndexBuffer(lineQuadIndexBuffer);
        }
        
        
        {
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
        ALAS_PROFILE_FUNCTION();
        texture->Bind();
        shader->Bind();
        shader->setMat4("u_viewProjectionMatrix", _Data.ViewProjectionMatrix);
        shader->setMat4("u_model", modelMatrix);
        shader->setVec4("u_Color", color.x, color.y, color.z, color.w);
        shader->setInt("u_Texture", 0);
        
		RenderCommand::DrawIndexed(_Data.quadVertexArray);
    }

    void Renderer::DrawLine(const glm::vec3& position, const float rotation, const glm::vec3& scale)
    {
        ALAS_PROFILE_FUNCTION();

        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position) *
            glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
            glm::scale(glm::mat4(1.0f), scale);
        
        _Data.lineShader->Bind();
        _Data.lineShader->setMat4("u_viewProjectionMatrix", _Data.ViewProjectionMatrix);
        _Data.lineShader->setMat4("u_model", modelMatrix);
        _Data.lineShader->setVec4("u_Color", 1.0f, 1.0f, 0.0f, 1.0f);

        RenderCommand::DrawLine(_Data.lineVertexArray);
    }

    void Renderer::DrawBox(const glm::vec3& position, const float rotation, const glm::vec3& scale)
    {
        ALAS_PROFILE_FUNCTION();

        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position) *
            glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
            glm::scale(glm::mat4(1.0f), scale);
        
        _Data.lineShader->Bind();
        _Data.lineShader->setMat4("u_viewProjectionMatrix", _Data.ViewProjectionMatrix);
        _Data.lineShader->setMat4("u_model", modelMatrix);
        _Data.lineShader->setVec4("u_Color", 1.0f, 0.0f, 0.0f, 1.0f);

        RenderCommand::DrawLine(_Data.lineQuadVertexArray);
    }
} // namespace AGS
