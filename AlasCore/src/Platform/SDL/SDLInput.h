#pragma once

#include "Core/Input.h"

namespace AGS {

    class SDLInput : public Input
    {
        protected:
            virtual bool IsKeyPressedImpl(int keyCode) override;
            virtual bool IsMouseButtonPressedImpl(int mouseCode) override;
            virtual float GetMousePosXImpl() override;
            virtual float GetMousePosYImpl() override;

        private:
            const bool * keys; 
            const bool * mouse;
            float mouseX, mouseY;
    };
}