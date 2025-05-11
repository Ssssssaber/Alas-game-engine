
#include "Core/Window.h"
#include "Core/LayerStack.h"
#include "Core/Time.h"

#include "Renderer/OrthCamera.h"

#include "Events/ApplicationEvent.h"

#include "Entity/Scene.h"

namespace Alas
{
    class GameLoop
    {
    public:
        GameLoop::GameLoop(Time* timeRef);
        ~GameLoop();

        bool GetIsRunning() { return _isRunning; }

        void Init(Shared<Scene> sceneRef);

        void Update();

        void Stop();

        void OnEvent(Event& e);

        bool OnWindowClose(WindowCloseEvent& event);
        bool OnWindowResize(WindowResizeEvent& event);
    
    private:
        void CopyScene(Shared<Scene> sceneRef);

    private:        
        Shared<Scene> _gameScene;
        Shared<OrthCamera> _camera;
        // Entity _cameraFollowEntity;

        Shared<Window> _window;
        Time* _time;

        LayerStack _layerStack;
        bool _isRunning = true;
    };
} // namespace Alas
