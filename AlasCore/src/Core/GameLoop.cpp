#include "GameLoop.h"

#include <chipmunk/chipmunk.h>

#include "Core/Input.h"
#include "Core/KeyCodes.h"

#include "Renderer/Renderer.h"
#include "Renderer/RendererCommand.h"

namespace Alas
{
    GameLoop::GameLoop(const Shared<Scene> sceneRef, Time* timeRef) : _scene(sceneRef), _time(timeRef)
    {
        _scene->Physics2DInit();

    }

    GameLoop::~GameLoop()
    {
        // _scene.pht
    }

    void GameLoop::Init()
    {
        _window.reset(Alas::Window::Create());
        _window->SetEventCallback(
            std::bind(&GameLoop::OnEvent, this, std::placeholders::_1)
        );
        _window->SetVSync(false);

        _camera.reset(new OrthCamera(-1.6f, 1.6f, -0.9f, 0.9f));
        // _camera->SetPosition({0.0f, 1.0f, 0.0f});
    }

    void GameLoop::Update()
    {
        static int count = 3;
        static float lastTime = 0;

        Window::SetCurrentWindow(*_window);
        RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        RenderCommand::Clear();

        Alas::Renderer::BeginScene(_camera);

        _scene->Physics2DUpdate();
        count += 1;
        if (count > 3)
        {
            float currentTime = Time::GetTimeInSeconds();
            _time->updateDeltaTime(currentTime - lastTime);
            lastTime = currentTime;
            _scene->RuntimeUpdate();
            count = 0;   
        }

        _scene->SceneUpdate();
        _window->OnUpdate();
        Renderer::EndScene();
    }

    void GameLoop::Stop()
    {

    }

    void GameLoop::OnEvent(Event& e)
    {
        EventDispatcher manager(e);
        manager.Dispatch<WindowCloseEvent>(
            std::bind(&GameLoop::OnWindowClose, this, std::placeholders::_1)
         );
    }

    bool GameLoop::OnWindowClose(WindowCloseEvent& event)
    {
        _scene->Physics2DStop();
        _isRunning = false;

        return true;
    }
} // namespace Alas
