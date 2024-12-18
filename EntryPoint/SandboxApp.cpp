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

            out vec4 o_Color;

            uniform mat4 u_viewProjectionMatrix;
            uniform mat4 u_model;

			void main()
			{
                o_Color = a_Color;
				gl_Position = u_viewProjectionMatrix * u_model * vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core

            layout(location = 0) out vec4 color;

            in vec4 o_Color;

			void main()
			{
				color = o_Color;
			}
		)";

		_shader.reset(new Alas::Shader(vertexSrc, fragmentSrc));
        _shader->Bind();

        Alas::Application::Get().GetWindow().SetVSync(false);

        float height = Alas::Application::Get().GetWindow().GetHeight();
        float width = Alas::Application::Get().GetWindow().GetWidth();
        
        camera = new Alas::OrthCamera(-1.6f, 1.6f, -0.9f, 0.9f);
    }

    void OnUpdate() override
    {           
        Alas::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        Alas::RenderCommand::Clear();

        

        float deltaTime = Alas::Time::getDeltaTime();
        float color = glm::sin(1 / 2) + 0.5f;

        if (Alas::Input::IsKeyPressed(ALAS_KEY_W))
        {
            _cameraPos.y += speed * deltaTime;
        }
        if (Alas::Input::IsKeyPressed(ALAS_KEY_S))
        {
            _cameraPos.y -= speed * deltaTime;
        }
        if (Alas::Input::IsKeyPressed(ALAS_KEY_A))
        {
            _cameraPos.x -= speed * deltaTime;
        }
        if (Alas::Input::IsKeyPressed(ALAS_KEY_D))
        {
            _cameraPos.x += speed * deltaTime;
        }

        if (Alas::Input::IsKeyPressed(ALAS_KEY_Q))
        {
            _cameraRotation -= rotation_speed * deltaTime;
        }
        if (Alas::Input::IsKeyPressed(ALAS_KEY_E))
        {
            _cameraRotation += rotation_speed * deltaTime;
        }

        camera->SetPosition(_cameraPos);
        camera->SetRotation(_cameraRotation);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, tri_pos);
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.5, 0.5, 0));
        _shader->setMat4("u_model", model);

        Alas::Renderer::BeginScene(camera);
        
        Alas::Renderer::Submit(_vertexArray, _shader);
        
        Alas::Renderer::EndScene();
    }

    void OnEvent(Alas::Event& event) override
    {
    
    }

    private:
        std::shared_ptr<Alas::Shader> _shader;
        std::shared_ptr<Alas::VertexArray> _vertexArray;
        glm::vec3 tri_pos = glm::vec3(0.0f);

        glm::vec3 _cameraPos = glm::vec3(0.0f, 0.0f, 1.0f);

        Alas::OrthCamera* camera;
        float _cameraRotation = 0;
        glm::vec3 _cameraPosition;
        float speed = 7.0f;
        float rotation_speed = 30.0f;
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
