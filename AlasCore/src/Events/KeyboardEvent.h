#pragma once 

#include "Event.h"

// KeyPressed, KeyReleased, KeyTyped,

namespace AGS {

    class KeyEvent : public Event
    {
    public:
        inline int GetKeyCode() const { return _keyCode; }      
        EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard)

    protected:
        KeyEvent(int keyCode) : _keyCode(keyCode) {}

        int _keyCode;
    };

    class KeyPressedEvent : public KeyEvent
    { 
    public:
        KeyPressedEvent(int keyCode, int repeatCount) 
            : KeyEvent(keyCode), _repeatedCount(repeatCount) {}

        inline int GetRepeatCount() const { return _repeatedCount; }

        EVENT_CLASS_TYPE(KeyPressed)
    private:
        int _repeatedCount;
    };

    class KeyReleasedEvent : public KeyEvent
    {
    public:
        KeyReleasedEvent(int keyCode) 
                : KeyEvent(keyCode) {}
        

        EVENT_CLASS_TYPE(KeyReleased)
    };

    
}