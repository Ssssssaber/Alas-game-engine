#pragma once
#include "Window.h"
#include "Input.h"

#include "Events/ApplicationEvent.h"
#include "Core/Layer.h"
#include "Core/LayerStack.h"
#include "ImGui/ImGuiLayer.h"

#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"

#include "Time.h"

// use pointers 
namespace Alas
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
        Input* _input;
        LayerStack _layerStack;

        Time* _time;

        bool _isRunning = true;
    };

    Application* CreateApplication();
} // namespace AGS

