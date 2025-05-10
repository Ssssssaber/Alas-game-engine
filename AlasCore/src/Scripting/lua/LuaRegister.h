#pragma once

#include <lua.hpp>
#include <sol.hpp>

#include "Core/KeyCodes.h"
#include "Core/Time.h"
#include "Core/Input.h"
#include "Entity/Entity.h"
#include "LuaComponents.h"
#include "LuaScriptHandle.h"

namespace Alas
{
    class LuaBasicFunctions
    {
    public:
        static void S_RegisterEntityRelatedFunctions(sol::state& lua, Entity entity);

        static void S_RisterBasicFunctionsForLuaState(sol::state& luaState);

        static void S_RegisterTypesForLuaState(sol::state& luaState);
        
        static float GetDeltaTime();

        static float GetElapsedTime();

        static bool IsButtonPressed(ALAS_Scancode keyCode);

        static glm::vec2 Normalize2(glm::vec2 vec);
        
        static glm::vec3 Normalize3(glm::vec3 vec);
        
        static glm::vec4 Normalize4(glm::vec4 vec);

        static void DestroyEntity(LuaEntity* entity);
    };

    
} // namespace Alas
