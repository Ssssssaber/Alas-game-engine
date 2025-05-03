#pragma once

#include "LuaRegister.h"
#include <set>
#define SCRIPTS_FILEPATH std::string("Assets/Scripts/")

namespace Alas {

    class ScriptingEngine 
    {
    public:
        static void Init();
        static void InitGameLoop();
        static void HandleScript(const std::string& filename, Entity entity);

        static void AddBeginCollisionUpdate(UID uid);
        static void AddEndCollisionUpdate(UID uid);
    
    private:
        static void LoadScript(const std::string& filename, Entity entity);
        static void ExecuteFunction(const std::string& functionName);

    private:
        static sol::state lua; 
        static std::set<UID> updatedScripts;
        static std::set<UID> beginCollisionUpdate;
        static std::set<UID> endCollisionUpdate;
    };
}

