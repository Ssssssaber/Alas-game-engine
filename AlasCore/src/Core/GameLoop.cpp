#include "GameLoop.h"

#include <chipmunk/chipmunk.h>

#include "Core/Input.h"
#include "Core/KeyCodes.h"

#include "Renderer/Renderer.h"
#include "Renderer/RendererCommand.h"

namespace Alas
{
    GameLoop::GameLoop(Shared<Scene> sceneRef, Time* timeRef) : _time(timeRef)
    {
        _scene.reset(new Scene());
        CopyScene(sceneRef);
        _scene->Physics2DInit();

    }

    GameLoop::~GameLoop()
    {
        // _scene.pht
    }

    void GameLoop::CopyScene(Shared<Scene> sceneRef)
    {
        ALAS_PROFILE_FUNCTION();

        auto entities = sceneRef->GetEntityMap();

        for (auto idAndEnt = entities.begin(); idAndEnt != entities.end(); idAndEnt++)
        {
            UID id = idAndEnt->first;
            Entity entToCopy = idAndEnt->second;

            Entity newEnt = _scene->CreateEntityWithId(entToCopy.GetComponent<TagComponent>().Tag, idAndEnt->first);

            auto& transform = newEnt.GetComponent<Transform>();
            auto& transformToCopy = entToCopy.GetComponent<Transform>();
            transform.Position = transformToCopy.Position;
            transform.Rotation = transformToCopy.Rotation;
            transform.Scale = transformToCopy.Scale;
        
            if (entToCopy.HasComponent<SpriteComponent>())
            {
                // implement sprite
                auto& sprite = newEnt.AddComponent<SpriteComponent>();

                auto& spriteToCopy = entToCopy.GetComponent<SpriteComponent>(); 
                
                sprite.c_Shader = spriteToCopy.c_Shader;
                sprite.c_Texture = spriteToCopy.c_Texture;
                sprite.Color = spriteToCopy.Color;
            }
            
            if (entToCopy.HasComponent<RigidBody2D>())
            {
                // implement sprite
                auto& rigidBody2D = newEnt.AddComponent<RigidBody2D>();

                auto& rigidBody2DToCopy = entToCopy.GetComponent<RigidBody2D>(); 
                
                rigidBody2D.Type = rigidBody2DToCopy.Type;
                rigidBody2D.Mass = rigidBody2DToCopy.Mass;
                rigidBody2D.GravityScale = rigidBody2DToCopy.GravityScale;
            }

            if (entToCopy.HasComponent<BoxCollider2D>())
            {
                // implement sprite
                auto& boxCollider2D = newEnt.AddComponent<BoxCollider2D>();

                auto& boxCollider2DToCopy = entToCopy.GetComponent<BoxCollider2D>(); 
                
                boxCollider2D.Offset = boxCollider2DToCopy.Offset;
                boxCollider2D.Size = boxCollider2DToCopy.Size;
            }

            if (entToCopy.HasComponent<NativeScriptComponent>())
            {
                // implement sprite
                auto& script = newEnt.AddComponent<NativeScriptComponent>();

                auto& scriptToCopy = entToCopy.GetComponent<NativeScriptComponent>(); 
                
                script.InstantiateScript = scriptToCopy.InstantiateScript;
                script.Instance = scriptToCopy.Instance;
                script.DestroyScript = scriptToCopy.DestroyScript;
            }

            if (entToCopy.HasComponent<LuaScriptComponent>())
            {
                // implement sprite
                auto& scriptToCopy = entToCopy.GetComponent<LuaScriptComponent>(); 
                auto& script = newEnt.AddComponent<LuaScriptComponent>(scriptToCopy.Filepath);
                script.Handle = LuaScriptHandle(std::make_shared<Entity>(newEnt));

                script.Filepath = scriptToCopy.Filepath;
            }
        }
    }

    void GameLoop::Init()
    {
        ALAS_PROFILE_FUNCTION();
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
        ALAS_PROFILE_FUNCTION();
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
