#include "Application.h"
#include "Events/ApplicationEvent.h"
#include "Core/Logger.h"
#include "SDL.h"
namespace AGS
{

    Application::Application() {}

    Application::~Application() {}
    
    void Application::Run()
    {
        // while (1);
        SDL_Window *window = nullptr;

        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            AGS_CORE_ERROR(SDL_GetError());
        }
        else
        {
            window = SDL_CreateWindow("Main Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
            if (window == NULL)
            {
                AGS_CORE_ERROR(SDL_GetError());
            }
            else
            {
                SDL_UpdateWindowSurface(window);
                SDL_Delay(200);
            }
        }
        
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
}