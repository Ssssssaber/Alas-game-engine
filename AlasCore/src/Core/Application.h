#pragma once
#include "Platform/SDL/SDLWindow.h"
#include "Events/ApplicationEvent.h"
#include "Core/Layer.h"
#include "Core/LayerStack.h"
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

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);
    
    private:
        Window* _window;
        LayerStack _layerStack;
        bool _isRunning;
    };

    Application* CreateApplication();
} // namespace AGS

