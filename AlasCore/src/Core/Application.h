#pragma once
#include "Platform/SDL/SDLWindow.h"
#include "Events/ApplicationEvent.h"
// use pointers 
namespace AGS
{
    class Application 
    {
    public:
        Application();

        virtual ~Application();
        
        void Run();

        void OnEvent(Event& e);

        bool OnWindowClose(WindowCloseEvent& event);
    
    private:
        Window* _window;
        bool _isRunning;
    };

    Application* CreateApplication();
} // namespace AGS

