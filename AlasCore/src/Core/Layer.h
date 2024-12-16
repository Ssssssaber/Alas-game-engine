#pragma once

#include "Events/Event.h"

namespace Alas {
    class Layer
    {
    public:
        Layer(const std::string debugName) : _debugName(debugName) {}
        virtual ~Layer() {}
        
        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate() {}
        virtual void OnImGuiRender() {}
        virtual void OnEvent(Event& event) {}
        // disable and enable layer func
    
        
        inline std::string GetDebugName() const { return _debugName; }

    private:
        std::string _debugName; 

    };
}