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

    void SDLGLWindow::OnUpdate()
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
            case SDL_MOUSEMOTION:
                // AGS_CORE_INFO("Mouse event");
                break;
            default:
                break;
            }
        }
        SDL_UpdateWindowSurface(_window);
    
        SDL_RenderPresent(_renderer);
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

    void SDLGLWindow::Init()
    {
        if (!s_IsSDLInitialized)
        {
            int success = SDL_Init(SDL_INIT_VIDEO);
            AGS_CORE_ERROR("SDL was not initialized: {0}", SDL_GetError());
            if (success < 0)
            {
                AGS_CORE_ERROR("SDL was not initialized: {0}", SDL_GetError());
                return;
            }
            s_IsSDLInitialized = true;
        }
        
        _window = SDL_CreateWindow(_params.title.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _params.width, _params.height, SDL_WINDOW_SHOWN);
        AGS_CORE_ERROR("keke {0} {1}", "is", "you");
        AGS_CORE_ERROR(SDL_GetError());
        AGS_CORE_ERROR("SDL was not initialized: {0}", SDL_GetError());
        if (_window == NULL)
        {
            AGS_CORE_ERROR("Window was not initialized: {0}", SDL_GetError());
            return;
        }
        _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
        AGS_CORE_ERROR("Window was not initialized: {0}", SDL_GetError());
        if (_renderer == NULL)
        {
            AGS_CORE_ERROR("Window was not initialized: {0}", SDL_GetError());
            return;
        }
        _context = SDL_GL_CreateContext(_window); 
        SDL_GL_MakeCurrent(_window, _context);

        SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
        SDL_RenderClear(_renderer);
    }

    void SDLGLWindow::ShutDown()
    {
        SDL_DestroyWindow(_window);
    }
}