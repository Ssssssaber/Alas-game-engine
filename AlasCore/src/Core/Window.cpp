#include "Window.h"
#include "Renderer/RendererCommand.h"
#include "Core/Input.h"

namespace Alas
{
    Window* Window::_currentWindow;
    Window* Window::_focusedWindow;

    void Window::SetCurrentWindow(Window& window)
    {
        RenderCommand::SetActiveWindow(window);
        Input::SetActiveWindow(window);
    }

    const Window& Window::GetCurrentWindow()
    {
        return *_currentWindow;    
    }

    const Window& Window::GetFocusedWindow()
    {
        return *_focusedWindow;    
    }
} // namespace Alas
