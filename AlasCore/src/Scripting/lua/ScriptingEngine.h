#pragma once

#include "LuaRegister.h"
#define SCRIPTS_FILEPATH std::string("Assets/Scripts/")

namespace Alas {

    class ScriptingEngine 
    {
    public:
        static void Init();    
        static void HandleScript(std::string filename, Entity entity);
    
    private:
        static void LoadScript(std::string filename, Entity entity);
        static void ExecuteScript();

    private:
        static sol::state lua; 
    };
}

