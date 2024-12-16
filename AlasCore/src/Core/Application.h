#pragma once
#include "Platform/SDL/SDLWindow.h"
#include "Platform/SDL/SDLInput.h"

#include "Events/ApplicationEvent.h"
#include "Core/Layer.h"
#include "Core/LayerStack.h"
#include "ImGui/ImGuiLayer.h"

#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"

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

        inline Window& GetWindow() { return *_window; }
        inline static Application& Get() { return *_instance; }
    
    private:
        static Application* _instance;

        ImGuiLayer* _imguiLayer;
        
        Window* _window;
        SDLInput* _input;
        LayerStack _layerStack;
        
        bool _isRunning = true;
    };

    Application* CreateApplication();
} // namespace AGS

