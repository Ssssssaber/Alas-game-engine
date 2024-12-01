#include "Application.h"
#include "Events/ApplicationEvent.h"
#include "Core/Logger.h"
#include "Input.h"
#include <glad/glad.h>
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

        _imguiLayer = new ImGuiLayer();
        PushOverlay(_imguiLayer);

        _input = new SDLInput();
        Input::Init();

        glGenVertexArrays(1, &_vertexArray);
		glBindVertexArray(_vertexArray);

		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};

		_vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		uint32_t indices[3] = { 0, 1, 2 };
		_indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
			}
		)";

		_shader.reset(new Shader(vertexSrc, fragmentSrc));
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
            glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            _shader->Bind();
            glBindVertexArray(_vertexArray);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

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