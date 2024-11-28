#include "Platform/SDL/SDLInput.h"
#include <SDL3/SDL.h>

namespace AGS
{
    bool SDLInput::IsKeyPressedImpl(int keyCode)
    {
        
        return keys[keyCode];
    }

    bool SDLInput::IsMouseButtonPressedImpl(int mouseCode)
    {
        
    }

    float SDLInput::GetMousePosXImpl() 
    {

    }

    float SDLInput::GetMousePosYImpl()
    {

    }    
} // namespace AGS
