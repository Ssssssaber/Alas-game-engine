#include "Application.h"

#include "Core/Logger.h"
#include "Core/GameLoop.h"

#include "Renderer/Renderer.h"

#include "Events/ApplicationEvent.h"

#include "Resources/ResourceManager.h"

namespace Alas
{
    Application* Application::_instance = nullptr;
    Application::Application()
    {
        ALAS_ASSERT(!_instance, "Application already exists");

        _instance = this;

        _window.reset(Alas::Window::Create());
        _window->SetEventCallback(
            std::bind(&Application::OnEvent, this, std::placeholders::_1)
        );

        ResourceManager::Init();
        ResourceManager::UpdateMetaFiles();
        
        _imguiLayer = new ImGuiLayer();
        PushOverlay(_imguiLayer);

        _input = Input::Init();

        _time = new Time();

        Renderer::Init();

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

    void Application::StartGameLoop(Shared<Scene> scene)
    {
        _gameLoop.reset(new GameLoop(_time));
        _gameLoop->Init(scene);
    }
    
    void Application::Run()
    {
        ALAS_PROFILE_FUNCTION()
        WindowResizeEvent e(1280, 720);
        ALAS_CORE_INFO(e.ToString());
        
        float lastTime = 0.0f;
        
        while (_isRunning)
        {

            ALAS_PROFILE_SCOPE("RunLoop");
            float currentTime = Time::GetTimeInSeconds();
            _time->updatePhysicsDeltaTime(currentTime - lastTime);
            lastTime = currentTime;
            
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

            Window::PollEvents();
            _window->OnUpdate();

            if (!_gameLoop) continue;
            
            if (_gameLoop->GetIsRunning())
            {
                _gameLoop->Update();
            }
            else
            {
                _gameLoop = nullptr;
            }
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