#include "Game.h"

#include "Core/Logger.h"
#include "Core/GameLoop.h"

#include "Renderer/Renderer.h"

#include "Resources/ResourceManager.h"
#include "Entity/SceneSerialization.h"

namespace Alas
{
    Game::Game()
    {
        ResourceManager::UpdateMetaFiles();

        _input = Input::Init();

        _time = new Time();

        _gameLoop.reset(new GameLoop(_time));
        
        ResourceManager::Init();
        Renderer::Init();
        _gameLoop->Init(SceneSerialization::DeserializeScene("Assets/main.yaml"));
    }

    Game::~Game() {}
    
    void Game::Run()
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

            Window::PollEvents(false);
            
            if (_gameLoop->GetIsRunning())
            {
                _gameLoop->Update();
            }
            else
            {
                _gameLoop = nullptr;
                _isRunning = false;
            }
        }
    }
}

int main(int argv, char** argc)
{
    Alas::Logger::Init();
    auto app = new Alas::Game();
    app->Run();
    delete app;

    return 0;
}