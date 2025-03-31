#pragma once

#include "LuaRegister.h"
#define SCRIPTS_FILEPATH std::string("Assets/Scripts/")

namespace Alas {

    class ScriptingEngine 
    {
    public:
        static void Init();    
        static void LoadAndExecuteScript(std::string filename, Entity entity);
    
    private:
        static sol::state lua; 
    };
}

