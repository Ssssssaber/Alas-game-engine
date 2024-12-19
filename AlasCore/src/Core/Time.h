#pragma once
#include "SDL3/SDL.h"

namespace Alas
{      
    class Time {
    
    public:
        Time() {}
        ~Time() {}

        void updateDeltaTime(float deltaTime) { _deltaTime = deltaTime; }
        static float GetTimeInSeconds() { return SDL_GetTicksNS() / 10e8; }
        static float getDeltaTime() { return _deltaTime; }

    private:
        static float _time;
        static float _deltaTime;
    };    
} // namespace AGS

