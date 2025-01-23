#pragma once
#include "Core/Window.h"
#include "Core/Input.h"

#include "Renderer/RendererCommand.h"

namespace Alas
{
    class WindowManager
    {
    public:
        static void SetActiveWindow(Window& window)
        {
            RenderCommand::SetActiveWindow(window);
            Input::SetActiveWindow(window);
        }
    private:
        static Window* _currentWindow;
    };   
} // namespace Alas
