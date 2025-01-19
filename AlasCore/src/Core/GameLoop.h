#include "Core/Window.h"
#include "GameObject/Scene.h"
#include "Core/Time.h"
#include "Core/LayerStack.h"
#include "Core/Input.h"

#include "Events/ApplicationEvent.h"
namespace Alas
{
    class GameLoop
    {
    public:
        GameLoop(const Shared<Scene> sceneRef);
        ~GameLoop() = default;

        void Init();

        void Update();

        void Stop();

        void OnEvent(Event& e);

        bool OnWindowClose(WindowCloseEvent& event);
    private:        
        Shared<Scene> _scene;
        std::vector<Shared<GameObject>> _gameObjects;
        Window* _window;
        Input* _input;
        Time* _time;

        LayerStack _layerStack;
        bool _isRunning = true;
    };
} // namespace Alas
