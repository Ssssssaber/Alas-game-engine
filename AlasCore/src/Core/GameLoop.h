
#include "Core/Window.h"
#include "Core/LayerStack.h"

#include "Renderer/OrthCamera.h"

#include "Events/ApplicationEvent.h"

#include "GameObject/Scene.h"

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
        
        Shared<OrthCamera> _camera;
        Shared<Window> _window;

        LayerStack _layerStack;
        bool _isRunning = true;
    };
} // namespace Alas
