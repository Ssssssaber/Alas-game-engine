#pragma once

#include "Event.h"

#include <sstream>
// unique namespace for each system
namespace AGS {

    class WindowResizeEvent : public Event
    {
        public:
            WindowResizeEvent(unsigned int width, unsigned int height) : _width(width), _height(height)
            {
            }
            // force inline       
            inline unsigned int GetWidth() const { return _width; }
            inline unsigned int GetHeight() const { return _height; }
            // to sting interface
            std::string ToString() const override
            {
                std::stringstream ss;

                ss << "WindowResizeEvent: " << _width << ", " << _height;

                return ss.str();
            }

            EVENT_CLASS_TYPE(WindowResize)
		    EVENT_CLASS_CATEGORY(EventCategoryApplication)

        private:
            unsigned int _width, _height;
    };

    class WindowCloseEvent : public Event
    {
    public:
        WindowCloseEvent() {}

        EVENT_CLASS_TYPE(WindowClose)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };
}