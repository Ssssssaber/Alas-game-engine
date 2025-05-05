#pragma once
#include "Core/Window.h"
#include "Core/Input.h"

#include "Core/Layer.h"
#include "Core/LayerStack.h"

#include "Events/ApplicationEvent.h"
#include "ImGui/ImGuiLayer.h"

#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"

#include "Core/Time.h"

// use pointers 
namespace Alas
{
    class GameLoop;
    class Scene;
    class Game 
    {
    public:
        Game();

        virtual ~Game();
        
        void Run();

        void StartGameLoop(Shared<Scene> scene);

    private:
        Alas::Shared<Alas::GameLoop> _gameLoop;
        Input* _input;
        Time* _time;
        bool _isRunning = true;
    };
} // namespace AGS

