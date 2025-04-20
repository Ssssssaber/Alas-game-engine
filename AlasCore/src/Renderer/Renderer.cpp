#include "Renderer.h"

#include "TextRendering.h"
#include "RendererCommand.h"

#include "Entity/Components.h"

namespace Alas
{
    Renderer::RendererData Renderer::_Data = RendererData();

    void Renderer::Init()
    {
        ALAS_PROFILE_FUNCTION()
        RenderCommand::EnableBlending();


        _Data.OverlayTextShader = Shader::Create("Assets/Shaders/OverlayText.shader");
        _Data.WorldSpaceTextShader = Shader::Create("Assets/Shaders/WorldSpaceText.shader");
        _Data.TextRenderer = TextRendering::Create();
        _Data.TextRenderer->Init();
        
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
                -100.0f, -100.0f, 
                100.0f, -100.0f, 
                100.0f,  100.0f, 
                -100.0f,  100.0f, 
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
            _Data.QuadVertexArray.reset(VertexArray::Create());
            
            float quadVertices[5 * 4] = {
                -100.0f, -100.0f, 0.0f, 0.0f, 0.0f,
                 100.0f, -100.0f, 0.0f, 1.0f, 0.0f,
                 100.0f,  100.0f, 0.0f, 1.0f, 1.0f,
                -100.0f,  100.0f, 0.0f, 0.0f, 1.0f
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
        
            _Data.QuadVertexArray->AddVertexBuffer(quadVertexBuffer);
            
            uint32_t quadIndices[6] = { 0, 1, 2, 2, 3, 0 };        
            Alas::Shared<Alas::IndexBuffer> quadIndexBuffer;
            quadIndexBuffer.reset(Alas::IndexBuffer::Create(quadIndices, sizeof(quadIndices) / sizeof(uint32_t)));
            
            _Data.QuadVertexArray->SetIndexBuffer(quadIndexBuffer);
        }
    }

    void Renderer::BeginScene(const Shared<OrthCamera>& camera)
	{
        ALAS_PROFILE_FUNCTION();
        _Data.Camera = camera;
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
        shader->setMat4("u_viewProjectionMatrix", _Data.Camera->GetViewProjectionMatrix());
        shader->setMat4("u_model", modelMatrix);
        shader->setVec4("u_Color", color.x, color.y, color.z, color.w);
        shader->setInt("u_Texture", 0);
        
		RenderCommand::DrawIndexed(_Data.QuadVertexArray);
    }


    void Renderer::DrawLine(const glm::vec3& position, const float rotation, const glm::vec3& scale)
    {
        ALAS_PROFILE_FUNCTION();

        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position) *
            glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
            glm::scale(glm::mat4(1.0f), scale);
        
        _Data.lineShader->Bind();
        _Data.lineShader->setMat4("u_viewProjectionMatrix", _Data.Camera->GetViewProjectionMatrix());
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
        _Data.lineShader->setMat4("u_viewProjectionMatrix", _Data.Camera->GetViewProjectionMatrix());
        _Data.lineShader->setMat4("u_model", modelMatrix);
        _Data.lineShader->setVec4("u_Color", 1.0f, 0.0f, 0.0f, 1.0f);

        RenderCommand::DrawLine(_Data.lineQuadVertexArray);
    }

    void Renderer::SubmitOverlayText(const std::string& text, const glm::vec2& position, float rotation, const glm::vec2& scale, glm::vec4 color)
    {
        _Data.OverlayTextShader->Bind();

        _Data.OverlayTextShader->setMat4("projection",
            _Data.Camera->GetProjectionMatrix()
        );
        _Data.OverlayTextShader->setVec3("textColor", color.x, color.y, color.z);

        _Data.TextRenderer->RenderText(text, {position.x, position.y, 0.0f}, scale);
    }

    void Renderer::SubmitWorldSpaceText(const std::string& text, const glm::vec3& position, float rotation, const glm::vec2& scale, glm::vec4 color)
    {
        
        _Data.WorldSpaceTextShader->Bind();
        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position) *
                glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
                glm::scale(glm::mat4(1.0f), glm::vec3(scale.x, scale.y, 1.0f));

        _Data.WorldSpaceTextShader->setMat4("u_viewProjectionMatrix",
            _Data.Camera->GetViewProjectionMatrix()
        );
        _Data.WorldSpaceTextShader->setMat4("u_model",
            modelMatrix
        );
        
        _Data.WorldSpaceTextShader->setVec4("textColor", color.x, color.y, color.z, color.w);

        _Data.TextRenderer->RenderText(text, position, scale);
        // Alas::Renderer::SubmitWorldSpaceText("not keke is not you", glm::vec3(50.0f), 1.0f, glm::vec2(1.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));

    }
} // namespace AGS
