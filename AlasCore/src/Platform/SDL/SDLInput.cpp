#include "Platform/SDL/SDLInput.h"


namespace Alas
{
    Input* Input::Init()
    {
        _instance = new SDLInput();
        _instance->InitImpl();
        
        return _instance;
    }
    
    void SDLInput::InitImpl()
    {
        keys = SDL_GetKeyboardState(NULL); 
        
    }

    bool SDLInput::IsKeyPressedImpl(int keyCode)
    {
        return keys[keyCode];
    }

    bool SDLInput::IsMouseButtonPressedImpl(int mouseCode)
    {
        mouse = SDL_GetMouseState(&mouseX, &mouseY);
        return mouse & SDL_BUTTON_MASK( mouseCode );
    }

    std::pair<float, float> SDLInput::GetMousePositionImpl()
    {
        mouse = SDL_GetMouseState(&mouseX, &mouseY);
        return std::pair<float, float>(mouseX, mouseY);
    }

    float SDLInput::GetMousePosXImpl() 
    {
        mouse = SDL_GetMouseState(&mouseX, &mouseY);
        return mouseX;
    }

    float SDLInput::GetMousePosYImpl()
    {
        mouse = SDL_GetMouseState(&mouseX, &mouseY);
        return mouseY;
    }
} // namespace AGS
