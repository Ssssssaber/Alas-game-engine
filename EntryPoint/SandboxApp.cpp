#include <Alas.h>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
class ExampleLayer : public AGS::Layer
{
public:
    ExampleLayer() : Layer("Example")
    {
        _vertexArray.reset(AGS::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 1.0f, 1.0f,
            0.0f,  0.5f, 0.0f, 0.5f, 0.5f, 1.0f, 1.0f
		};
        
        std::shared_ptr<AGS::VertexBuffer> vertexBuffer; 
		vertexBuffer.reset(AGS::VertexBuffer::Create(vertices, sizeof(vertices)));
        
        {
            AGS::BufferLayout layout {
                {AGS::ShaderElementType::Float3, "a_Position"},
                {AGS::ShaderElementType::Float4, "a_Color"}
            };
            vertexBuffer->SetLayout(layout);
        }

        _vertexArray->AddVertexBuffer(vertexBuffer);
		
		uint32_t indices[3] = { 0, 1, 2 };        
        std::shared_ptr<AGS::IndexBuffer> indexBuffer;
		indexBuffer.reset(AGS::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        
        _vertexArray->SetIndexBuffer(indexBuffer);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            uniform mat4 u_translation;

			void main()
			{
				gl_Position = u_translation * vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core

            layout(location = 0) out vec4 color;

            uniform vec4 u_Color;

			void main()
			{
				color = u_Color;
			}
		)";

		_shader.reset(new AGS::Shader(vertexSrc, fragmentSrc));
        _shader->Bind();

        AGS::Application::Get().GetWindow().SetVSync(false);
    }

    void OnUpdate() override
    {           
        AGS::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        AGS::RenderCommand::Clear();

        AGS::Renderer::BeginScene();

        float time = SDL_GetTicks();
        float color = glm::sin(1 / 2) + 0.5f;
        _shader->setVec4("u_Color", color, 0.0f, 0.0f, 1.0f);

    

        if (AGS::Input::IsKeyPressed(AGS_KEY_W))
        {
            AGS_CLIENT_TRACE("w pressed");
            tri_speed.y += 0.1f;
        }
        else if (AGS::Input::IsKeyPressed(AGS_KEY_S))
        {
            
            AGS_CLIENT_TRACE("s pressed");
            tri_speed.y -= 0.1f;
        }
        else if (AGS::Input::IsKeyPressed(AGS_KEY_A))
        {
            
            AGS_CLIENT_TRACE("a pressed");
            tri_speed.x -= 0.1f;
        }
        else if (AGS::Input::IsKeyPressed(AGS_KEY_D))
        {
            
            AGS_CLIENT_TRACE("d pressed");
            tri_speed.x += 0.1f;
        }

        glm::mat4 translation = glm::mat4(1.0f);
        translation = glm::translate(translation, tri_speed);
        translation = glm::rotate(translation, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        translation = glm::scale(translation, glm::vec3(0.5, 0.5, 0.5));
        _shader->setMat4("u_translation", translation);

        _shader->Bind();
        _vertexArray->Bind();
        
        AGS::Renderer::Submit(_vertexArray);
        
        AGS::Renderer::EndScene();
    }

    void OnEvent(AGS::Event& event) override
    {
    
    }

    private:
        std::shared_ptr<AGS::Shader> _shader;
        std::shared_ptr<AGS::VertexArray> _vertexArray;
        glm::vec3 tri_speed = glm::vec3(0.0f);
        float speed = 0.1f;
};

class Sandbox : public AGS::Application
{
public:
    Sandbox()
    {
        PushLayer(new ExampleLayer());
        // PushOverlay(new AGS::ImGuiLayer());
    }

    ~Sandbox()
    {

    }
};


AGS::Application* AGS::CreateApplication()
{
    return new Sandbox();
}
