#pragma once

namespace AGS {

    class Input
    {
    public:
        Input() { _instance = this; }
        static void Init() { _instance->InitImpl(); }
        inline static bool IsKeyPressed(int keycode) { return _instance->IsKeyPressedImpl(keycode); }
        inline static bool IsMouseButtonPressed(int mouseCode) { return _instance->IsMouseButtonPressedImpl(mouseCode); }
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
    // private:
        static Input* _instance;

    };
}