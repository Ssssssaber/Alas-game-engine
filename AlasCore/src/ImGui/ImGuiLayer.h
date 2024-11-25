#pragma once

#include "Core/Layer.h"

#include "Events/ApplicationEvent.h"
#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"

namespace AGS {

    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        void OnAttach();
        void OnDetach();

        void OnUpdate();
        void OnEvent(Event& event);
    private:
        // KeyPressed, KeyReleased, KeyTyped,
        // WindowSizeChanged
        //MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
        bool OnKeyPressedEvent(KeyPressedEvent& event);
        bool OnKeyReleasedEvent(KeyReleasedEvent& event);
        bool OnKeyTypedEvent(KeyTypedEvent& event);
        bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& event);
        bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& event);
        bool OnMouseMovedEvent(MouseMovedEvent& event);
        bool OnMouseScrolledEvent(MouseScrolledEvent& event);
        bool OnWindowSizeChangedEvent(WindowResizeEvent& event);
        
        float _time = 0.0f;
    };
}