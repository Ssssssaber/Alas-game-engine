

#include "SDLWindow.h"
#include "Events/Event.h"
#include <Events/ApplicationEvent.h>
#include <Events/MouseEvent.h>
#include <Events/KeyboardEvent.h>
#include "imgui_impl_sdl3.h"
namespace AGS {

    static bool s_IsSDLInitialized = false;

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
        ShutDown();
    }
    
    void SDLGLWindow::Init()
    {
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        if (!s_IsSDLInitialized)
        {
            int success = SDL_Init(SDL_INIT_VIDEO);
            // AGS_CORE_ERROR("SDL was not initialized: {0}", SDL_GetError());
            // AGS_ASSERT(!success, SDL_GetError())
                
            if (success < 0)
            {
                AGS_CORE_ERROR("SDL was not initialized: {0}", SDL_GetError());
                return;
            }
            s_IsSDLInitialized = true;
        }
        
        _window = SDL_CreateWindow(_params.title.data(), _params.width, _params.height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        if (_window == NULL)
        {
            AGS_CORE_ERROR("Window was not initialized: {0}", SDL_GetError());
            return;
        }

        _context = SDL_GL_CreateContext(_window); 
        int status = gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress);
        AGS_ASSERT(status, "GLAD was not initialized")
        
        AGS_CORE_INFO("OpenGL {0}.{1}", GLVersion.major, GLVersion.minor);
        AGS_CORE_INFO("Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
        AGS_CORE_INFO("Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
        AGS_CORE_INFO("Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

        SDL_GL_MakeCurrent(_window, _context);
    }

    void SDLGLWindow::ShutDown()
    {
        SDL_DestroyWindow(_window);
    }

    void SDLGLWindow::OnUpdate()
    {
        SDL_Event e;
        while (SDL_PollEvent (&e) != 0) {
            ImGui_ImplSDL3_ProcessEvent(&e);
            if (e.type == SDL_EVENT_QUIT)
            {
                WindowCloseEvent event;
                _params.EventCallback(event);
                ShutDown();
                break;
            }
            else 
            {
                ProcessEvents(e);
            }
        }
        SDL_GL_SwapWindow(_window);
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
                SDL_Keymod mods = SDL_GetModState();
                KeyPressedEvent e(sdlEvent.key.key, sdlEvent.key.scancode, sdlEvent.key.mod, 1);
                _params.EventCallback(e);
                break;
            }
            case SDL_EVENT_KEY_UP:
            {
                SDL_Keymod mods = SDL_GetModState();
                KeyReleasedEvent e(sdlEvent.key.key, sdlEvent.key.scancode, sdlEvent.key.mod);
                _params.EventCallback(e);
                break;
            }
            case SDL_EVENT_TEXT_INPUT:
            {
                SDL_Keymod mods = SDL_GetModState();
                KeyTypedEvent e(sdlEvent.key.key, sdlEvent.key.scancode, sdlEvent.key.mod, sdlEvent.text.text);
                _params.EventCallback(e);
                break;
            }
            case SDL_EVENT_WINDOW_RESIZED:
            {
                glViewport(0, 0, sdlEvent.window.data1, sdlEvent.window.data2);
            }
        }
    }
}