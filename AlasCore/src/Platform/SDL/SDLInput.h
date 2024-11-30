#pragma once

#include "Core/Input.h"
#include <SDL3/SDL.h>
namespace AGS {

    class SDLInput : public Input
    {
        protected:
            virtual void InitImpl() override;
            virtual bool IsKeyPressedImpl(int keyCode) override;
            virtual bool IsMouseButtonPressedImpl(int mouseCode) override;
            virtual std::pair<float, float> GetMousePositionImpl() override;
            virtual float GetMousePosXImpl() override;
            virtual float GetMousePosYImpl() override;

        private:
            const bool * keys; 
            SDL_MouseButtonFlags mouse;
            float mouseX, mouseY;
    };
}