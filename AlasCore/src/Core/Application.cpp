#include "Application.h"
#include "Events/ApplicationEvent.h"
#include "Core/Logger.h"
#include "Input.h"
#include "Renderer/BufferLayout.h"
#include "Renderer/Renderer.h"
#include "Renderer/RendererCommand.h"

namespace Alas
{
    Application* Application::_instance = nullptr;
    Application::Application()
    {
        ALAS_ASSERT(!_instance, "Application already exists");
        _instance = this;

        _window = Alas::Window::Create();
        _window->SetEventCallback(
            std::bind(&Application::OnEvent, this, std::placeholders::_1)
        );

        _imguiLayer = new ImGuiLayer();
        PushOverlay(_imguiLayer);

        _input = new SDLInput();
        Input::Init();
    }

    Application::~Application() {}

    void Application::OnEvent(Event& e)
    {
        EventDispatcher manager(e);
        manager.Dispatch<WindowCloseEvent>(
           std::bind(&Application::OnWindowClose, this, std::placeholders::_1)
        );

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
        ALAS_CORE_INFO(e.ToString());
        while (_isRunning)
        {
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