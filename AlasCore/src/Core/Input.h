#pragma once
#include "Core/Window.h"
#include <utility>
namespace Alas {

    class Input
    {
    public:
        Input() { _instance = this; }
        static Input* Init();
        inline static void SetActiveWindow(Window& window) { _window = &window; }
        inline static bool IsKeyPressed(int keycode) { return _instance->IsKeyPressedImpl(keycode) && _window == Window::GetFocusedWindow(); }
        inline static bool IsMouseButtonPressed(int mouseCode) { return _instance->IsMouseButtonPressedImpl(mouseCode) && _window == Window::GetFocusedWindow(); }
        inline static std::pair<float, float> GetMousePosition() { return _instance->GetMousePositionImpl(); }
        inline static float GetMousePosX() { return _instance->GetMousePosXImpl(); }
        inline static float GetMousePosY() { return _instance->GetMousePosYImpl(); }
    protected:
        virtual void InitImpl() = 0;
        virtual bool IsKeyPressedImpl(int keyCode) = 0;
        virtual bool IsMouseButtonPressedImpl(int mouseCode) = 0;
        virtual std::pair<float, float> GetMousePositionImpl() = 0;
        virtual float GetMousePosXImpl() = 0;
        virtual float GetMousePosYImpl() = 0;
    private:
        static Input* _instance;
        static Window* _window;

    };
}