#include "GameLoop.h"
#include "Renderer/Renderer.h"
#include "Renderer/RendererCommand.h"

namespace Alas
{
    GameLoop::GameLoop(const Shared<Scene> sceneRef)
    {
        _scene = sceneRef;

        for (auto IdGameObject : _scene->GetGameObjectList())
        {
            auto goPtr = IdGameObject.second;
            _gameObjects.push_back(goPtr);
        }
    }

    void GameLoop::Init()
    {
        _window = Alas::Window::Create();
        _window->SetEventCallback(
            std::bind(&GameLoop::OnEvent, this, std::placeholders::_1)
        );
        _window->SetVSync(false);
    }

    void GameLoop::Update()
    {
        RenderCommand::SetActiveWindow(*_window);
        RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        RenderCommand::Clear();
        for (auto go : _gameObjects)
        {
            go->Update();
            go->InnerUpdate();
            Renderer::Submit(*go);
        }
        
        _window->OnUpdate();
    }

    void GameLoop::Stop()
    {

    }

    void GameLoop::OnEvent(Event& e)
    {

    }

    bool GameLoop::OnWindowClose(WindowCloseEvent& event)
    {

    }
} // namespace Alas
