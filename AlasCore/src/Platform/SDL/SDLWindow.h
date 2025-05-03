#pragma once

#include "Core/Window.h"
#include <SDL3/SDL.h>
struct SDL_Window;
namespace Alas{

    class SDLGLWindow : public Window 
    {
        friend class Window;
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
            uint32_t windowID;

            uint32_t width;
            uint32_t height;

            float aspectRatio = 1.6f / 0.9f;
            
            bool isVsync;

            EventCallbackFunction EventCallback;

        } _params;


        SDL_Window* _window;
        
    };
}