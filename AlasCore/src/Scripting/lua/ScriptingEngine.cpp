#include "ScriptingEngine.h"

namespace Alas 
{
    sol::state ScriptingEngine::lua = sol::state();

    void ScriptingEngine::Init()
    {
        lua.open_libraries(sol::lib::base, sol::lib::io, sol::lib::math, sol::lib::table);
        S_RegisterTypesForLuaState(lua);
        S_RisterBasicFunctionsForLuaState(lua);
        
    }

    void ScriptingEngine::HandleScript(std::string filename, Entity entity)
    {
        LoadScript(filename, entity);
        ExecuteScript();
    }

    void ScriptingEngine::LoadScript(std::string filename, Entity entity)
    {
        ALAS_PROFILE_FUNCTION()
        try
        {
            S_RegisterEntityRelatedFunctions(lua, entity);
            lua.script_file(SCRIPTS_FILEPATH + filename);
        }
        catch (const sol::error& e)
        {
            ALAS_CORE_ERROR(e.what());
            return;
        }
    }

    void ScriptingEngine::ExecuteScript()
    {
        ALAS_PROFILE_FUNCTION()
        auto result = lua["Update"]();

        if (!result.valid())
        {
            sol::error e = result;
            ALAS_CORE_ERROR("{0}", e.what());
        }
    }
}