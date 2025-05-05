

#include "SDLWindow.h"

#include <glad/glad.h>
#include <SDL3/SDL_opengl.h>
#include <imgui_impl_sdl3.h>

#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"
#include "Events/KeyboardEvent.h"

#include "Core/Input.h"
#include "Core/KeyCodes.h"
namespace Alas {

    static bool s_IsSDLInitialized = false;
    static bool s_IsGLADInitialized = false;
    std::unordered_map<SDL_WindowID, SDLGLWindow*> s_windows;

    static SDL_GLContext _context = nullptr;

    Window* Window::Create(const WindowParams& params)
    {
        return new SDLGLWindow(params);
    }
    
    SDLGLWindow::SDLGLWindow(const WindowParams &params)
    {
        _params.title = params.Title;
        _params.width = params.Width;
        _params.height = params.Height;
        
        Init();
    }

    SDLGLWindow::~SDLGLWindow()
    {
        s_windows.erase(_params.windowID);
        ShutDown();
    }
    
    void SDLGLWindow::Init()
    {
        if (!s_IsSDLInitialized)
        {
            SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
            SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

            int success = SDL_Init(SDL_INIT_VIDEO);
                
            if (success < 0)
            {
                ALAS_CORE_ERROR("SDL was not initialized: {0}", SDL_GetError());
                return;
            }
            s_IsSDLInitialized = true;
        }
        
        _window = SDL_CreateWindow(_params.title.data(), _params.width, _params.height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

        if (_window == NULL)
        {
            ALAS_CORE_ERROR("Window was not initialized: {0}", SDL_GetError());
            return;
        }
        _params.windowID = SDL_GetWindowID(_window);
        s_windows[_params.windowID] = this;

        if (!s_IsGLADInitialized)
        {
            _context = SDL_GL_CreateContext(_window);
            
            int status = gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress);
            if (status < 0) 
            {
                ALAS_ASSERT(status, "GLAD was not initialized")
                return;
            }
            
            ALAS_CORE_INFO("OpenGL {0}.{1}", GLVersion.major, GLVersion.minor);
            ALAS_CORE_INFO("Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
            ALAS_CORE_INFO("Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
            ALAS_CORE_INFO("Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

            s_IsGLADInitialized = true;
        }

        SDL_GL_MakeCurrent(_window, _context);
    }

    void SDLGLWindow::ShutDown()
    {
        SDL_DestroyWindow(_window);
    }

    void Window::PollEvents(bool ImGuiEnabled)
    {
        _focusedWindow = s_windows[SDL_GetWindowID(SDL_GetKeyboardFocus())];

        SDL_Event e;
        while (SDL_PollEvent (&e) != 0) {
            if (ImGuiEnabled) ImGui_ImplSDL3_ProcessEvent(&e);

            SDLGLWindow* window = s_windows[e.window.windowID];
            if (e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED || e.type == SDL_EVENT_QUIT)
            {
                if (!window) continue;
                
                WindowCloseEvent event;

                window->_params.EventCallback(event);
                window->ShutDown();
            }
            else 
            {
                if (window) window->ProcessEvents(e);
            }
        }
    }

    void SDLGLWindow::OnUpdate()
    {
        SDL_PumpEvents();
        SDL_GL_SwapWindow(_window);
    }

    void SDLGLWindow::SetAsCurrent() const
    {
        SDL_GL_MakeCurrent(_window, _context);
    }

    void SDLGLWindow::SetVSync(bool enabled)
    {
        if (enabled) SDL_GL_SetSwapInterval(1);
        else SDL_GL_SetSwapInterval(0);

        _params.isVsync = enabled;
    }

    bool SDLGLWindow::IsVSync()
    {
        return _params.isVsync;
    }

    void SDLGLWindow::ProcessEvents(SDL_Event& sdlEvent) 
    {
        switch(sdlEvent.type)
        {
            //MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
            {
                SDL_Keymod mods = SDL_GetModState();
                MouseButtonPressedEvent e(sdlEvent.button.button);
                _params.EventCallback(e);
                break;
            }
            case SDL_EVENT_MOUSE_BUTTON_UP:
            {
                SDL_Keymod mods = SDL_GetModState();
                MouseButtonReleasedEvent e(sdlEvent.button.button);
                _params.EventCallback(e);
                break;
            }
            case SDL_EVENT_MOUSE_MOTION:
            {
                MouseMovedEvent e(sdlEvent.button.x, sdlEvent.button.y);
                _params.EventCallback(e);
                break;
            }
            case SDL_EVENT_MOUSE_WHEEL:
            {
                MouseScrolledEvent e(sdlEvent.wheel.y, sdlEvent.wheel.y);
                _params.EventCallback(e);
                break;
            }
            // KeyPressed, KeyReleased, KeyTyped,
            case SDL_EVENT_KEY_DOWN:
            {
                KeyPressedEvent e(sdlEvent.key.key, sdlEvent.key.scancode, sdlEvent.key.mod, 1);
                _params.EventCallback(e);
                break;
            }
            case SDL_EVENT_KEY_UP:
            {
                KeyReleasedEvent e(sdlEvent.key.key, sdlEvent.key.scancode, sdlEvent.key.mod);
                _params.EventCallback(e);
                break;
            }
            case SDL_EVENT_TEXT_INPUT:
            {
                KeyTypedEvent e(sdlEvent.key.key, sdlEvent.key.scancode, sdlEvent.key.mod, sdlEvent.text.text);
                _params.EventCallback(e);
                break;
            }
            case SDL_EVENT_WINDOW_RESIZED:
            {
                // glViewport(0, 0, sdlEvent.window.data1, sdlEvent.window.data2 * (1 / _params.aspectRatio));
                glViewport(0, 0, sdlEvent.window.data1, sdlEvent.window.data2);
            }
        }
    }
}