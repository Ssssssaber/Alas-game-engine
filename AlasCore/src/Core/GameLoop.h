
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
        GameLoop(const Shared<Scene> sceneRef, Time* timeRef);
        ~GameLoop();

        bool GetIsRunning() { return _isRunning; }

        void Init();

        void Update();

        void Stop();

        void OnEvent(Event& e);

        bool OnWindowClose(WindowCloseEvent& event);
    private:        
        Shared<Scene> _scene;
        Shared<OrthCamera> _camera;
        Shared<Window> _window;
        Time* _time;

        LayerStack _layerStack;
        bool _isRunning = true;
    };
} // namespace Alas
