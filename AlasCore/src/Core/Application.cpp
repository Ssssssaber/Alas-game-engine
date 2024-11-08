#include "Application.h"
#include "Events/ApplicationEvent.h"
#include "Core/Logger.h"


namespace AGS
{

    Application::Application() 
    {
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
    }
    
    void Application::Run()
    {
        WindowResizeEvent e(1280, 720);
        AGS_CORE_INFO(e.ToString());
        while (_isRunning)
        {
            _window->OnUpdate();
        }
        SDL_Delay(2000);
    }

    bool Application::OnWindowClose(WindowCloseEvent& event)
    {
        _isRunning = false;
        return true;
    }
}