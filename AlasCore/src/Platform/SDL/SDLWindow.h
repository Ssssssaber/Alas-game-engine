#pragma once
#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

#include "Core/Window.h"
struct SDL_Window;

namespace Alas{

    class SDLGLWindow : public Window 
    {
    public:
        SDLGLWindow(const WindowParams &params = WindowParams());
        ~SDLGLWindow();

        void OnUpdate() override;

        virtual uint32_t GetWidth() const override { return _params.width; }
        virtual uint32_t GetHeight() const override { return _params.height; }
        
        virtual void SetAsCurrent() const override;

        void SetEventCallback(const EventCallbackFunction& callback) override { _params.EventCallback = callback; }
        void SetVSync(bool enabled) override;
        bool IsVSync() override;    

    private:
        void Init();
        void ShutDown();
        void ProcessEvents(SDL_Event& sdlEvent);

        struct SDLParams {
            std::string title;
            Uint32 windowID;

            uint32_t width;
            uint32_t height;
            
            bool isVsync;

            EventCallbackFunction EventCallback;

        } _params;
        

        SDL_Window* _window;
        static SDL_GLContext  _context;
    };
}