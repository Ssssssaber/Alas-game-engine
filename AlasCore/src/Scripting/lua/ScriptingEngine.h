#pragma once

#include "LuaRegister.h"
#include <unordered_map>
#include <set>
namespace Alas {

    class ScriptingEngine 
    {
    public:
        static void Init();
        static void InitGameLoop();
        static void HandleScript(const std::string& filename, const Entity& entity);
        static void LoadScript(const std::string& filename, const Entity& entity);

        static void AddBeginCollisionUpdate(UID first, UID second);
        static void AddEndCollisionUpdate(UID first, UID second);
    
    private:
        static void ExecuteFunction(const std::string& functionName);
        static void ExecuteFunction(const std::string& functionName, const Entity& entity);

    private:
        static sol::state lua; 
        static std::set<UID> updatedScripts;
        static std::unordered_map<UID, std::set<UID>> beginCollisionUpdate;
        static std::unordered_map<UID, std::set<UID>> endCollisionUpdate;
        // static std::unordered_map<UID, LuaEntity> luaEntityMap;
    };
}

