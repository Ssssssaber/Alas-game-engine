#pragma once 

#include "Event.h"

// KeyPressed, KeyReleased, KeyTyped,

namespace AGS {

    class KeyEvent : public Event
    {
    public:
        inline int GetKeyCode() const { return _keyCode; }      
        inline int GetScanCode() const { return _scanCode; }
        EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard)

    protected:
        KeyEvent(int keyCode, int scanCode) : _keyCode(keyCode), _scanCode(scanCode) {}

        int _keyCode;
        int _scanCode;
    };

    class KeyPressedEvent : public KeyEvent
    { 
    public:
        KeyPressedEvent(int keyCode, int scanCode, int repeatCount) 
            : KeyEvent(keyCode, scanCode), _repeatedCount(repeatCount) {}

        inline int GetRepeatCount() const { return _repeatedCount; }

        EVENT_CLASS_TYPE(KeyPressed)
    private:
        int _repeatedCount;
    };

    class KeyReleasedEvent : public KeyEvent
    {
    public:
        KeyReleasedEvent(int keyCode, int scanCode) 
                : KeyEvent(keyCode, scanCode) {}
        

        EVENT_CLASS_TYPE(KeyReleased)
    };

    class KeyTypedEvent : public KeyEvent
    {
    public:
        KeyTypedEvent(int keyCode, int scanCode) 
                : KeyEvent(keyCode, scanCode) {}
        

        EVENT_CLASS_TYPE(KeyReleased)
    };
}