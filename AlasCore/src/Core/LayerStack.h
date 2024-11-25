#pragma once

#include "Core/Core.h"
#include "Core/Layer.h"

namespace AGS {

    class LayerStack 
    {
    public:
        LayerStack();
        ~LayerStack();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* overlay);
        
        std::vector<Layer*>::iterator begin() { return _layers.begin(); }
        std::vector<Layer*>::iterator end() { return _layers.end(); }
        
    private:
        std::vector<Layer*> _layers;
        std::vector<Layer*>::iterator _layerInsert;
    };
}