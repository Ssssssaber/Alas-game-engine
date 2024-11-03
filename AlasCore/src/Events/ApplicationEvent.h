#pragma once

#include "Event.h"

#include <sstream>

namespace AGS {

    class WindowResizeEvent : public Event
    {
        public:
            WindowResizeEvent(unsigned int width, unsigned int height) : _width(width), _height(height)
            {
            }

            inline unsigned int GetWidth() const { return _width; }
            inline unsigned int GetHeight() const {return _height; }

            std::string ToString() const override
            {
                std::stringstream ss;

                ss << "WindowResizeEvent: " << _width << ", " << _height;

                return ss.str();
            }

            EVENT_CLASS_TYPE(WindowResize)
		    EVENT_CLASS_CATEGORY(EventCategoryApplication)

            // static EventType GetStaticType() { return EventType::WindowResize; } 
            // virtual EventType GetEventType() const override { return GetStaticType(); }
            // virtual const char* GetName() const override { return "WindowResize"; }
            // virtual int GetCategoryFlags() const override { return EventCategory::EventCategoryApplication; }
        private:
            unsigned int _width, _height;
    };
}