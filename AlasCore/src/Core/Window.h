#pragma once

#include "Events/Event.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

namespace AGS
{

    struct WindowParams
    {
        std::string Title;
        uint32_t Width;
        uint32_t Height;

        WindowParams(std::string title = "Alas Game Engine", uint32_t width = 1280, uint32_t height = 720) :
            Title(title), Width(width), Height(height)
        {

        }   
        
    };

    class Window
    {
    public:
        using EventCallbackFunction = std::function<void(Event&)>;

        virtual void OnUpdate() = 0;

        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;
        
        virtual void SetEventCallback(const EventCallbackFunction& callback) = 0;
        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsVSync() = 0;

        // virtual void* GetNativeWindow() const = 0;

        static Window* Create(const WindowParams& params = WindowParams());
    };

}