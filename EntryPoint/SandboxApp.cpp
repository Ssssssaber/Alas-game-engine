#include <Alas.h>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "gtx/string_cast.hpp"
#include <gtc/type_ptr.hpp>
#include "imgui.h"

class ExampleLayer : public Alas::Layer
{
public:
    ExampleLayer() : Layer("Example")
    {
        Alas::Application::Get().GetWindow().SetVSync(false);

        float height = Alas::Application::Get().GetWindow().GetHeight();
        float width = Alas::Application::Get().GetWindow().GetWidth();
        
        camera = new Alas::OrthCamera(-1.6f, 1.6f, -0.9f, 0.9f);
                /*
            TRIANGLE GAMEOBJECT
        */
        Alas::VertexArray* triVertexArray = Alas::VertexArray::Create();

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f,
            0.0f,  0.5f, 0.0f, 0.5f, 0.5f, 1.0f, 0.5f
		};
        
        std::shared_ptr<Alas::VertexBuffer> triVertexBuffer; 
		triVertexBuffer.reset(Alas::VertexBuffer::Create(vertices, sizeof(vertices)));
        
        {
            Alas::BufferLayout layout {
                {Alas::ShaderElementType::Float3, "a_Position"},
                {Alas::ShaderElementType::Float4, "a_Color"}
            };
            triVertexBuffer->SetLayout(layout);
        }

        triVertexArray->AddVertexBuffer(triVertexBuffer);
		
		uint32_t triIndices[3] = { 0, 1, 2 };        
        std::shared_ptr<Alas::IndexBuffer> triIndexBuffer;
		triIndexBuffer.reset(Alas::IndexBuffer::Create(triIndices, sizeof(triIndices) / sizeof(uint32_t)));
        
        triVertexArray->SetIndexBuffer(triIndexBuffer);

        

        /*
            QUAD GAMEOBJECT
        */
        Alas::VertexArray* quadVertexArray = Alas::VertexArray::Create();

		float quadVertices[3 * 4] = {
			-1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f
        };
        
        std::shared_ptr<Alas::VertexBuffer> quadVertexBuffer; 
		quadVertexBuffer.reset(Alas::VertexBuffer::Create(quadVertices, sizeof(quadVertices)));
        
        {
            Alas::BufferLayout layout {
                {Alas::ShaderElementType::Float3, "a_Position"}
            };
            quadVertexBuffer->SetLayout(layout);
        }

        quadVertexArray->AddVertexBuffer(quadVertexBuffer);
		
		uint32_t quadIndices[6] = { 0, 1, 2, 2, 3, 0 };        
        std::shared_ptr<Alas::IndexBuffer> quadIndexBuffer;
		quadIndexBuffer.reset(Alas::IndexBuffer::Create(quadIndices, sizeof(quadIndices) / sizeof(uint32_t)));
        
        quadVertexArray->SetIndexBuffer(quadIndexBuffer);

        /*
            SHADERS
        */

		std::string baseVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;
            
            out vec3 o_Position;
            out vec4 o_Color;

            uniform mat4 u_viewProjectionMatrix;
            uniform mat4 u_model;

			void main()
			{
                o_Position = a_Position;
                o_Color = a_Color;
				gl_Position = u_viewProjectionMatrix * u_model * vec4(a_Position, 1.0);	
			}
		)";

		std::string baseFragmentSrc = R"(
			#version 330 core

            layout(location = 0) out vec4 color;

            in vec3 o_Position;
            in vec4 o_Color;

            uniform vec4 u_Color;

			void main()
			{
				color = u_Color;
			}
		)";     

		std::string quadVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

            out vec3 o_Position;

            uniform mat4 u_viewProjectionMatrix;
            uniform mat4 u_model;

			void main()
			{
                o_Position = a_Position;
				gl_Position = u_viewProjectionMatrix * u_model * vec4(a_Position, 1.0);	
			}
		)";

            // uniform vec4 u_Color;
		std::string quadFragmentSrc = R"(
			#version 330 core

            layout(location = 0) out vec4 color;

            in vec3 o_Position;

			void main()
			{
				color = vec4(0.1, 0.3, 0.6, 1.0);
			}
		)";

		

        /*
            GAMEOBJECTS
        */
        Alas::Shader* triangleShader = new Alas::Shader(baseVertexSrc, baseFragmentSrc);   
        triangle = new Alas::GameObject(triVertexArray, triangleShader);
        
                
        for (int i = 0; i < 20; i++)
        {
            for (int j = 0; j < 20; j++)
            {
                Alas::Shader* goShader = new Alas::Shader(baseVertexSrc, baseFragmentSrc);
                auto go = new Alas::GameObject(quadVertexArray, goShader);
                glm::vec4 color = glm::normalize(glm::vec4(i, j, glm::abs(i - j), i + j));
                go->GetShader()->setVec4("u_Color", color.x, color.y, color.z, color.w);
                go->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
                go->SetPosition(glm::vec3(i * 0.3f, j * 0.3f, 0.0f));
                gameObjects.push_back(go);
            }
        }

        Alas::Shader* quadShader = new Alas::Shader(baseVertexSrc, baseFragmentSrc);
        go = new Alas::GameObject(quadVertexArray, quadShader);

        Alas::Shader* trShader = new Alas::Shader(baseVertexSrc, baseFragmentSrc);
        auto tr = new Alas::GameObject(triVertexArray, trShader);
        tr->SetRotation(glm::vec3(0.0f, 0.0f, 45.0f));
        tr->GetShader()->setVec4("u_Color", 1.0f, 0.3f, 0.4f, 1.0f);

        // gameObjects.push_back(go); gameObjects.push_back(triangle); gameObjects.push_back(tr);  
    }

    void OnUpdate() override
    {           
        Alas::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        Alas::RenderCommand::Clear();
        
        float deltaTime = Alas::Time::getDeltaTime();

        if (Alas::Input::IsKeyPressed(ALAS_KEY_I))
        {
            _tri_pos.y += triangleSpeed * deltaTime;
        }
        if (Alas::Input::IsKeyPressed(ALAS_KEY_K))
        {
            _tri_pos.y -= triangleSpeed * deltaTime;
        }
        if (Alas::Input::IsKeyPressed(ALAS_KEY_J))
        {
            _tri_pos.x -= triangleSpeed * deltaTime;
        }
        if (Alas::Input::IsKeyPressed(ALAS_KEY_L))
        {
            _tri_pos.x += triangleSpeed * deltaTime;
        }

        triangle->SetPosition(_tri_pos);

        if (Alas::Input::IsKeyPressed(ALAS_KEY_W))
        {
            _cameraPos.y += cameraSpeed * deltaTime;
        }
        if (Alas::Input::IsKeyPressed(ALAS_KEY_S))
        {
            _cameraPos.y -= cameraSpeed * deltaTime;
        }
        if (Alas::Input::IsKeyPressed(ALAS_KEY_A))
        {
            _cameraPos.x -= cameraSpeed * deltaTime;
        }
        if (Alas::Input::IsKeyPressed(ALAS_KEY_D))
        {
            _cameraPos.x += cameraSpeed * deltaTime;
        }

        if (Alas::Input::IsKeyPressed(ALAS_KEY_Q))
        {
            _cameraRotation -= cameraRotationSpeed * deltaTime;
        }
        if (Alas::Input::IsKeyPressed(ALAS_KEY_E))
        {
            _cameraRotation += cameraRotationSpeed * deltaTime;
        }

        camera->SetPosition(_cameraPos);
        camera->SetRotation(_cameraRotation);

        Alas::Renderer::BeginScene(camera);

        // ALAS_CLIENT_INFO("{0}; {1}", glm::to_string(triangle->GetPosition()), glm::to_string(go->GetPosition()));

        std::vector<Alas::GameObject*>::iterator it;
        for (it = gameObjects.begin(); it != gameObjects.end(); it++)
        {
            Alas::GameObject *go = (*it);
            Alas::Renderer::Submit(go->GetVertexArray(), go->GetShader(), go->GetModelMatrix());
        }
        
        go->GetShader()->setVec4("u_Color", _quadColor.x, _quadColor.y, _quadColor.z, 1.0f);
        Alas::Renderer::Submit(go->GetVertexArray(), go->GetShader(), go->GetModelMatrix());
        triangle->GetShader()->setVec4("u_Color", _triColor.x, _triColor.y, _triColor.z, 1.0f);
        Alas::Renderer::Submit(triangle->GetVertexArray(), triangle->GetShader(), triangle->GetModelMatrix());
        
        Alas::Renderer::EndScene();
    }

    void OnImGuiRender()
    {
        ImGui::Begin("Color picker");
        ImGui::ColorEdit3("Triangle color", glm::value_ptr(_triColor));
        ImGui::ColorEdit3("Square color", glm::value_ptr(_quadColor));
        ImGui::End();
    }

    void OnEvent(Alas::Event& event) override
    {
    
    }

    private:
        Alas::GameObject* go;
        Alas::GameObject* triangle;

        std::vector<Alas::GameObject*> gameObjects;

        glm::vec3 _tri_pos = glm::vec3(0.0f);

        glm::vec3 _quadColor = { 0.5f, 0.1f, 0.3f};
        glm::vec3 _triColor = { 0.2f, 0.6f, 0.8f};

        float triangleSpeed = 5.0f;
        float triangleRotationSpeed = 1.0f;

        Alas::OrthCamera* camera;
        glm::vec3 _cameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
        float _cameraRotation = 0;

        float cameraSpeed = 7.0f;
        float cameraRotationSpeed = 30.0f;
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
