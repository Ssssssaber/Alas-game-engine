#include "GameLoop.h"

#include <chipmunk/chipmunk.h>

#include "Core/Input.h"
#include "Core/KeyCodes.h"

#include "Renderer/Renderer.h"
#include "Renderer/RendererCommand.h"

#include "Scripting/lua/ScriptingEngine.h"

#define FIXED_DELTA_TIME 1.0f / 60.0f
namespace Alas
{
    GameLoop::GameLoop(Time* timeRef) : _time(timeRef)
    {
        _window.reset(Alas::Window::Create());
        _window->SetEventCallback(
            std::bind(&GameLoop::OnEvent, this, std::placeholders::_1)
        );
        _window->SetVSync(false);
        // _camera->SetPosition(glm::vec3(-500.0f, -500.0f, 0.0f));

    }

    GameLoop::~GameLoop()
    {
        // _scene.pht
    }

    void GameLoop::CopyScene(Shared<Scene> sceneRef)
    {
        ALAS_PROFILE_FUNCTION();

        _gameScene->_gravity = sceneRef->_gravity;

        auto entities = sceneRef->GetEntityMap();

        for (auto idAndEnt = entities.begin(); idAndEnt != entities.end(); idAndEnt++)
        {
            UID id = idAndEnt->first;
            Entity entToCopy = idAndEnt->second;

            Entity newEnt = _gameScene->CreateEntityWithId(entToCopy.GetComponent<TagComponent>().Tag, idAndEnt->first);

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

            if (entToCopy.HasComponent<OverlayText>())
            {
                // implement sprite
                auto& textToCopy = entToCopy.GetComponent<OverlayText>(); 
                auto& text = newEnt.AddComponent<OverlayText>(textToCopy.DisplayText, textToCopy.Color);
                text.ScreenPosition = textToCopy.ScreenPosition;
                text.Rotation = textToCopy.Rotation;
                text.Scale = textToCopy.Scale;
            }

            if (entToCopy.HasComponent<WorldSpaceText>())
            {
                // implement sprite
                auto& textToCopy = entToCopy.GetComponent<WorldSpaceText>(); 
                auto& text = newEnt.AddComponent<WorldSpaceText>(textToCopy.DisplayText, textToCopy.Color);
                text.Offset = textToCopy.Offset;
                text.Rotation = textToCopy.Rotation;
                text.Scale = textToCopy.Scale;
            }

            if (entToCopy.HasComponent<CameraComponent>())
            {
                auto& transform = entToCopy.GetComponent<Transform>();
                // _camera->SetPosition(transform.Position);
                // _camera->SetRotation(transform.Rotation.z);

            }
        }
    }

    void GameLoop::Init(Shared<Scene> sceneRef)
    {
        ALAS_PROFILE_FUNCTION();

        ScriptingEngine::Init();

        _gameScene.reset(new Scene());
        _camera.reset(new OrthCamera(_window->GetWidth(), _window->GetHeight()));
        CopyScene(sceneRef);
        _gameScene->BOX_PHYSICS_SCALE = sceneRef->BOX_PHYSICS_SCALE;
        _gameScene->GameLoopInit();
    }

    void GameLoop::FixedUpdate(float deltaTime)
    {   
        static float s_timePassed = 0.0f; 
        s_timePassed += deltaTime;

        if (s_timePassed < FIXED_DELTA_TIME) return;

        s_timePassed = 0.0f;
        _gameScene->PhysicsUpdate(FIXED_DELTA_TIME);
    }

    void GameLoop::Update()
    {
        ALAS_PROFILE_FUNCTION();

        Window::SetCurrentWindow(*_window);
        RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        RenderCommand::Clear();

        Alas::Renderer::BeginScene(_camera);

        FixedUpdate(_time->GetDeltaTime());
        _gameScene->RuntimeUpdate();
        
        _gameScene->RenderUpdate();
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

        manager.Dispatch<WindowResizeEvent>(
            std::bind(&GameLoop::OnWindowResize, this, std::placeholders::_1)
        );
    }

    bool GameLoop::OnWindowClose(WindowCloseEvent& event)
    {
        _gameScene->Physics2DStop();
        _isRunning = false;

        return true;
    }

    bool GameLoop::OnWindowResize(WindowResizeEvent& event)
    {
        _camera->SetNewSize(event.GetWidth(), event.GetHeight());
        return true;
    }
} // namespace Alas
