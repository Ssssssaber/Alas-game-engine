#include "Renderer.h"

#include "TextRendering.h"
#include "RendererCommand.h"

#include "Entity/Components.h"
#include "Core/Window.h"

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
        ALAS_PROFILE_FUNCTION()
        texture->Bind();
        shader->Bind();
        shader->setMat4("u_viewProjectionMatrix", _Data.Camera->GetViewProjectionMatrix());
        shader->setMat4("u_model", modelMatrix);
        shader->setVec4("u_Color", color.x, color.y, color.z, color.w);
        shader->setInt("u_Texture", 0);
        
		RenderCommand::DrawIndexed(_Data.QuadVertexArray);
    }

    void Renderer::SubmitOverlayText(const std::string& text, const glm::vec2& position, float rotation, const glm::vec2& scale, glm::vec4 color)
    {
        _Data.OverlayTextShader->Bind();

        _Data.OverlayTextShader->setMat4("projection",
            glm::ortho(
                0.0f,
                static_cast<float>(Window::GetCurrentWindow().GetWidth()),
                0.0f,
                static_cast<float>(Window::GetCurrentWindow().GetHeight())
            )
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
