#include "Application.h"
#include "Events/ApplicationEvent.h"
#include "Core/Logger.h"

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
    }

    Application::~Application() {}

    void Application::OnEvent(Event& e)
    {
        EventDispatcher manager(e);
        manager.Dispatch<WindowCloseEvent>(
           std::bind(&Application::OnWindowClose, this, std::placeholders::_1)
        );

        AGS_CORE_INFO("Event {0}: ", e.ToString());

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
            glClearColor(0.7f, 0.9f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            for (Layer* layer : _layerStack)
            {
                layer->OnUpdate();
            }

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