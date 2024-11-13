

#include "SDLWindow.h"
#include "Events/Event.h"
#include <Events/ApplicationEvent.h>


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
            AGS_ASSERT(!success, "SDL was not initialized: {0}", SDL_GetError())
                
            if (success < 0)
            {
                AGS_CORE_ERROR("SDL was not initialized: {0}", SDL_GetError());
                return;
            }
            s_IsSDLInitialized = true;
        }
        
        _window = SDL_CreateWindow(_params.title.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _params.width, _params.height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
        if (_window == NULL)
        {
            AGS_CORE_ERROR("Window was not initialized: {0}", SDL_GetError());
            return;
        }

        SDL_GLContext context = SDL_GL_CreateContext(_window); 
        int status = gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress);
        AGS_ASSERT(status, "GLAD was not initialized")
        
        AGS_CORE_INFO("OpenGL {0}.{1}", GLVersion.major, GLVersion.minor);
        AGS_CORE_INFO("Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
        AGS_CORE_INFO("Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
        AGS_CORE_INFO("Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

        SDL_GL_MakeCurrent(_window, context);
    }

    void SDLGLWindow::ShutDown()
    {
        SDL_DestroyWindow(_window);
    }

    void SDLGLWindow::OnUpdate()
    {
        ProcessEvents();
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

    void SDLGLWindow::ProcessEvents()
    {
        SDL_Event e;
        while (SDL_PollEvent (&e) != 0) {
            switch (e.type)
            {
            case SDL_QUIT:
            {
                WindowCloseEvent event;
                _params.EventCallback(event);
                ShutDown();
                break;
            }
            // case SDL_
            case SDL_MOUSEMOTION:
                break;
            default:
                break;
            }
        }
    }
}