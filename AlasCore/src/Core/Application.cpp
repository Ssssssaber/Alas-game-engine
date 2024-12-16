#include "Application.h"
#include "Events/ApplicationEvent.h"
#include "Core/Logger.h"
#include "Input.h"
#include "Renderer/BufferLayout.h"
#include "SDL3/SDL.h"
#include "Renderer/Renderer.h"
#include "Renderer/RendererCommand.h"

namespace AGS
{
    Application* Application::_instance = nullptr;
    Application::Application()
    {
        AGS_ASSERT(!_instance, "Application already exists");
        _instance = this;

        _window = AGS::Window::Create();
        _window->SetEventCallback(
            std::bind(&Application::OnEvent, this, std::placeholders::_1)
        );

        _window->SetVSync(true);

        _imguiLayer = new ImGuiLayer();
        PushOverlay(_imguiLayer);

        _input = new SDLInput();
        Input::Init();

        _vertexArray.reset(VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 1.0f, 1.0f,
            0.0f,  0.5f, 0.0f, 0.5f, 0.5f, 1.0f, 1.0f
		};
        
        std::shared_ptr<VertexBuffer> vertexBuffer; 
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
        
        {
            BufferLayout layout {
                {ShaderElementType::Float3, "a_Position"},
                {ShaderElementType::Float4, "a_Color"}
            };
            vertexBuffer->SetLayout(layout);
        }

        _vertexArray->AddVertexBuffer(vertexBuffer);
		
		uint32_t indices[3] = { 0, 1, 2 };        
        std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        
        _vertexArray->SetIndexBuffer(indexBuffer);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;


            uniform vec4 u_Color;

			out vec3 v_Position;
            out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
                v_Color = u_Color;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
            in vec4 v_Color;

			void main()
			{
				color = v_Color;
			}
		)";

		_shader.reset(new Shader(vertexSrc, fragmentSrc));
        _shader->Bind();
    }

    Application::~Application() {}

    void Application::OnEvent(Event& e)
    {
        EventDispatcher manager(e);
        manager.Dispatch<WindowCloseEvent>(
           std::bind(&Application::OnWindowClose, this, std::placeholders::_1)
        );

        // AGS_CORE_INFO("Event {0}: ", e.ToString());

        for (auto it = _layerStack.end(); it != _layerStack.begin();)
        {
            (*--it)->OnEvent(e);
            if (e.Handled())
            {
                break;
            }
        }

    }
    
    void Application::Run()
    {
        WindowResizeEvent e(1280, 720);
        AGS_CORE_INFO(e.ToString());
        while (_isRunning)
        {
            RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			RenderCommand::Clear();

			Renderer::BeginScene();
            float time = SDL_GetTicks();
            float color = glm::sin(1 / 2) + 0.5f;
            _shader->setVec4("u_Color", color, 0.0f, 0.0f, 1.0f);
            _shader->Bind();
            _vertexArray->Bind();
            
            Renderer::Submit(_vertexArray);
			
            Renderer::EndScene();

            for (Layer* layer : _layerStack)
            {
                layer->OnUpdate();
            }

            _imguiLayer->Begin();
            for (Layer* layer : _layerStack)
            {
                layer->OnImGuiRender();
            }
            _imguiLayer->End();

            _window->OnUpdate();
        }
        SDL_Delay(2000);
    }

    bool Application::OnWindowClose(WindowCloseEvent& event)
    {
        _isRunning = false;
        return true;
    }
    
    void Application::PushLayer(Layer* layer)
    {
        _layerStack.PushLayer(layer);
        layer->OnAttach();
    }
    void Application::PushOverlay(Layer* layer)
    {
        _layerStack.PushOverlay(layer);
        layer->OnAttach();
    }
}