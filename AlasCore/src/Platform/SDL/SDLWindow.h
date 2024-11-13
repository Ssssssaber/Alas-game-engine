#pragma once
#include <glad/glad.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include "Core/Window.h"
struct SDL_Window;

namespace AGS{

    class SDLGLWindow : public Window 
    {
    public:
        SDLGLWindow(const WindowParams &params = WindowParams());
        ~SDLGLWindow();

        void OnUpdate() override;

        uint32_t GetWidth() const override { return _params.width; }
        uint32_t GetHeight() const override { return _params.height; }
        
        void SetEventCallback(const EventCallbackFunction& callback) override { _params.EventCallback = callback; }
        void SetVSync(bool enabled) override;
        bool IsVSync() override;

    private:
        void Init();
        void ShutDown();

        struct SDLParams {
            std::string title;
            uint32_t width;
            uint32_t height;
            
            bool isVsync;

            EventCallbackFunction EventCallback;

        } _params;
        

        SDL_Window* _window;
    };
}