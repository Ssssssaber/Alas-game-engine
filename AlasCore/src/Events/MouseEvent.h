#pragma once 

#include "Event.h"

//MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
namespace Alas {
    
    class MouseButtonPressedEvent : public Event
    {
    public:
        MouseButtonPressedEvent(int buttonCode) : _buttonCode(buttonCode) {}
        inline int GetKeyCode() const { return _buttonCode; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonPressedEvent: " << _buttonCode;

            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonPressed)
        EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouseButton)
    private:
        int _buttonCode;
    };

    
    class MouseButtonReleasedEvent : public Event
    {
    public:
        MouseButtonReleasedEvent(int buttonCode) : _buttonCode(buttonCode) {}
        inline int GetKeyCode() const { return _buttonCode; }
        
        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonReleasedEvent: " << _buttonCode;

            return ss.str();
        }
        
        EVENT_CLASS_TYPE(MouseButtonReleased)
        EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouseButton)
    private:
        int _buttonCode;
    };

    
    class MouseMovedEvent : public Event
    {
    public:
        MouseMovedEvent(float posX, float posY) 
        : _posX(posX), _posY(posY) {}

        inline float GetX() const { return _posX; }
        inline float GetY() const { return _posY; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseEvent: " << _posX << ", " << _posY;

            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseMoved)
        EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
    private: 
        float _posX, _posY;
    };

    class MouseScrolledEvent : public Event
    {
    public:
        MouseScrolledEvent(float offsetX, float offsetY) {}

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseScrolledEvent: " << _offsetX << ", " << _offsetY;

            return ss.str();
        }
        
        inline float GetXOffset() { return _offsetX; }
        inline float GetYOffset() { return _offsetY; }

        EVENT_CLASS_TYPE(MouseScrolled)
        EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
    private:
        float _offsetX, _offsetY;
    };
}