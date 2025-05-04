#include "ScriptingEngine.h"

namespace Alas 
{
    sol::state ScriptingEngine::lua = sol::state();
    std::set<UID> ScriptingEngine::updatedScripts;
    std::set<UID> ScriptingEngine::beginCollisionUpdate;
    std::set<UID> ScriptingEngine::endCollisionUpdate;

    void ScriptingEngine::Init()
    {
        lua.open_libraries(sol::lib::base, sol::lib::io, sol::lib::math, sol::lib::table);
        S_RegisterTypesForLuaState(lua);
        S_RisterBasicFunctionsForLuaState(lua);
    }

    void ScriptingEngine::InitGameLoop()
    {
        updatedScripts.clear();
        beginCollisionUpdate.clear();
        endCollisionUpdate.clear();
    }

    void ScriptingEngine::HandleScript(const std::string& filename, Entity entity)
    {
        LoadScript(filename, entity);
        
        if (updatedScripts.find(entity.GetUID()) == updatedScripts.end())
        {
            // TODO: check if oncreate exists
            ExecuteFunction("OnCreate");
            updatedScripts.insert(entity.GetUID());
        }

        if (beginCollisionUpdate.find(entity.GetUID()) != beginCollisionUpdate.end())
        {
            auto& luaHandle = entity.GetComponent<LuaScriptComponent>();
            if (luaHandle._beginCollisionFunctionName != LUA_SCRIPT_NO_COLLISION_FUNC)
            {
                ExecuteFunction(luaHandle._beginCollisionFunctionName);
                beginCollisionUpdate.erase(entity.GetUID());
            }
        }

        if (endCollisionUpdate.find(entity.GetUID()) != endCollisionUpdate.end())
        {
            auto& luaHandle = entity.GetComponent<LuaScriptComponent>();
            if (luaHandle._endCollisionFunctionName != LUA_SCRIPT_NO_COLLISION_FUNC)
            {
                ExecuteFunction(luaHandle._endCollisionFunctionName);
                endCollisionUpdate.erase(entity.GetUID());
            }
        }
        
        ExecuteFunction("Update");


    }

    void ScriptingEngine::LoadScript(const std::string& filename, Entity entity)
    {
        ALAS_PROFILE_FUNCTION()
        try
        {
            S_RegisterEntityRelatedFunctions(lua, entity);
            lua.script_file(filename);
        }
        catch (const sol::error& e)
        {
            ALAS_CORE_ERROR(e.what());
            return;
        }
    }

    void ScriptingEngine::ExecuteFunction(const std::string& functionName)
    {
        ALAS_PROFILE_FUNCTION()
        auto result = lua[functionName]();

        if (!result.valid())
        {
            sol::error e = result;
            ALAS_CORE_ERROR("{0}", e.what());
        }
    }

    void ScriptingEngine::AddBeginCollisionUpdate(UID uid)
    {
        ALAS_PROFILE_FUNCTION();
        beginCollisionUpdate.insert(uid);
    }
    
    void ScriptingEngine::AddEndCollisionUpdate(UID uid)
    {
        ALAS_PROFILE_FUNCTION();
        endCollisionUpdate.insert(uid);
    }
}