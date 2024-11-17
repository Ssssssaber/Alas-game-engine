#pragma once

#include "Events/Event.h"

namespace AGS {
    class Layer
    {
    public:
        Layer(std::string debugName) : _debugName(debugName) {}
        virtual ~Layer() {}
        
        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate() {}
        virtual void OnEvent(Event& event) {}
        // disable and enable layer func
    
        
        inline std::string GetDebugName() const { return _debugName; }

    private:
        std::string _debugName; 

    };
}