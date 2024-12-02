#pragma once

#include "Core/Window.h"
#include "SDL.h"
namespace AGS{

    class SDLGLWindow : public Window 
    {
    public:
        SDLGLWindow(const WindowParams &params = WindowParams());
        ~SDLGLWindow();

        void OnUpdate() override;

        virtual uint32_t GetWidth() const override { return _params.width; }
        virtual uint32_t GetHeight() const override { return _params.height; }
        
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
        SDL_GLContext _context;
        SDL_Renderer* _renderer;
    };
}