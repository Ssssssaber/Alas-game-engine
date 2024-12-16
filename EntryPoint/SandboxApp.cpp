#include <Alas.h>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
class ExampleLayer : public Alas::Layer
{
public:
    ExampleLayer() : Layer("Example")
    {
        _vertexArray.reset(Alas::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 1.0f, 1.0f,
            0.0f,  0.5f, 0.0f, 0.5f, 0.5f, 1.0f, 1.0f
		};
        
        std::shared_ptr<Alas::VertexBuffer> vertexBuffer; 
		vertexBuffer.reset(Alas::VertexBuffer::Create(vertices, sizeof(vertices)));
        
        {
            Alas::BufferLayout layout {
                {Alas::ShaderElementType::Float3, "a_Position"},
                {Alas::ShaderElementType::Float4, "a_Color"}
            };
            vertexBuffer->SetLayout(layout);
        }

        _vertexArray->AddVertexBuffer(vertexBuffer);
		
		uint32_t indices[3] = { 0, 1, 2 };        
        std::shared_ptr<Alas::IndexBuffer> indexBuffer;
		indexBuffer.reset(Alas::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        
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

		_shader.reset(new Alas::Shader(vertexSrc, fragmentSrc));
        _shader->Bind();

        Alas::Application::Get().GetWindow().SetVSync(false);
    }

    void OnUpdate() override
    {           
        Alas::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        Alas::RenderCommand::Clear();

        Alas::Renderer::BeginScene();

        float deltaTime = Alas::Time::getDeltaTime();
        float color = glm::sin(1 / 2) + 0.5f;
        _shader->setVec4("u_Color", color, 0.0f, 0.0f, 1.0f);

        ALAS_CLIENT_TRACE("{0}", deltaTime);

        if (Alas::Input::IsKeyPressed(ALAS_KEY_W))
        {
            ALAS_CLIENT_TRACE("w pressed");
            tri_speed.y += speed * deltaTime;
        }
        else if (Alas::Input::IsKeyPressed(ALAS_KEY_S))
        {
            
            ALAS_CLIENT_TRACE("s pressed");
            tri_speed.y -= speed * deltaTime;
        }
        else if (Alas::Input::IsKeyPressed(ALAS_KEY_A))
        {
            
            ALAS_CLIENT_TRACE("a pressed");
            tri_speed.x -= speed * deltaTime;
        }
        else if (Alas::Input::IsKeyPressed(ALAS_KEY_D))
        {
            
            ALAS_CLIENT_TRACE("d pressed");
            tri_speed.x += speed * deltaTime;
        }


        glm::mat4 translation = glm::mat4(1.0f);
        translation = glm::translate(translation, tri_speed);
        translation = glm::rotate(translation, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        translation = glm::scale(translation, glm::vec3(0.5, 0.5, 0.5));
        _shader->setMat4("u_translation", translation);

        _shader->Bind();
        _vertexArray->Bind();
        
        Alas::Renderer::Submit(_vertexArray);
        
        Alas::Renderer::EndScene();
    }

    void OnEvent(Alas::Event& event) override
    {
    
    }

    private:
        std::shared_ptr<Alas::Shader> _shader;
        std::shared_ptr<Alas::VertexArray> _vertexArray;
        glm::vec3 tri_speed = glm::vec3(0.0f);
        float speed = 7.0f;
};

class Sandbox : public Alas::Application
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


Alas::Application* Alas::CreateApplication()
{
    return new Sandbox();
}
