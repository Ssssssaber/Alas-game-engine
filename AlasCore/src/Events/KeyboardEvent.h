#pragma once 

#include "Event.h"

// KeyPressed, KeyReleased, KeyTyped,

namespace Alas {

    class KeyEvent : public Event
    {
    public:
        inline int GetKeyCode() const { return _keyCode; }      
        inline int GetScanCode() const { return _scanCode; }
        inline int GetKeyMod() const { return _mod; }
        EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard)

    protected:
        KeyEvent(int keyCode, int scanCode, int mod) : _keyCode(keyCode), _scanCode(scanCode), _mod(mod) {}

        int _keyCode;
        int _scanCode;
        int _mod;
    };

    class KeyPressedEvent : public KeyEvent
    { 
    public:
        KeyPressedEvent(int keyCode, int scanCode, int mod, int repeatCount) 
            : KeyEvent(keyCode, scanCode, mod), _repeatedCount(repeatCount) {}

        inline int GetRepeatCount() const { return _repeatedCount; }
        std::string ToString() const override
        {
            std::stringstream ss;

            ss << "KeyPressedEvent: " << _keyCode << ", " << _scanCode;

            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyPressed)
    private:
        int _repeatedCount;
    };

    class KeyReleasedEvent : public KeyEvent
    {
    public:
        KeyReleasedEvent(int keyCode, int scanCode, int mod) 
                : KeyEvent(keyCode, scanCode, mod) {}

        std::string ToString() const override
        {
            std::stringstream ss;

            ss << "KeyReleasedEvent: " << _keyCode << ", " << _scanCode;

            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyReleased)
    };

    class KeyTypedEvent : public KeyEvent
    {
    public:
        KeyTypedEvent(int keyCode, int scanCode, int mod, const char* text) 
                : KeyEvent(keyCode, scanCode, mod), _text(text)
        {
            
        }
        
        std::string ToString() const override
        {
            std::stringstream ss;

            ss << "KeyTypedEvent: " << _keyCode << ", " << _scanCode;

            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyTyped)
        const char* _text;
    };
}