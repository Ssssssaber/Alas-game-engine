#pragma once

#include <string>
#include <functional>

#include "Core/Core.h"

namespace Alas {

    enum class EventType
    {
        None = 0,
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
        AppTick, AppUpdate, AppRender,
        KeyPressed, KeyReleased, KeyTyped,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
    };

    enum EventCategory
    {
        None = 0,
        EventCategoryApplication    =BIT(0),
        EventCategoryInput          =BIT(1),
        EventCategoryKeyboard       =BIT(2),
        EventCategoryMouse          =BIT(3),
        EventCategoryMouseButton    =BIT(4)
    };
    
    class Event
    {
        friend class EventDispatcher;
    
    public:
        
        virtual EventType GetEventType() const { return EventType::None; }
        virtual const char* GetName() const { return "Event"; };
        virtual int GetCategoryFlags() const { return EventCategory::None; };
        virtual bool Handled() const { return _handled; }
        virtual std::string ToString() const { return GetName(); }

        inline bool isInCategory(EventCategory category)
        {
            return GetCategoryFlags() & category;
        }
    protected:
        bool _handled = false;
    };

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; } \
								virtual EventType GetEventType() const override { return GetStaticType(); } \
								virtual const char* GetName() const override { return #type; }
#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

    class EventDispatcher
    {
        template<typename T>
        using EventFn = std::function<bool(T&)>;
        public:
            EventDispatcher(Event& event) : _event(event)
            {

            }
            
            template<typename T>
            bool Dispatch(EventFn<T> func)
            {
                if (_event.GetEventType() == T::GetStaticType())
                {
                    _event._handled = func(*(T*)&_event);
                    return true;
                }
                return false;
            }
        private:
            Event& _event;
    };

    inline std::ostream& operator<<(std::ostream& os, const Event& e)
    {
        return os << e.ToString();
    }
}