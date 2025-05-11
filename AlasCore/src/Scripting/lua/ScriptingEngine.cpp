#include "ScriptingEngine.h"

namespace Alas 
{
    sol::state ScriptingEngine::lua = sol::state();
    std::set<UID> ScriptingEngine::updatedScripts;
    std::unordered_map<UID, std::set<UID>> ScriptingEngine::beginCollisionUpdate;
    std::unordered_map<UID, std::set<UID>> ScriptingEngine::endCollisionUpdate;
    std::unordered_map<UID, LuaEntity> ScriptingEngine::LuaEntityMap;

    void ScriptingEngine::Init()
    {
        lua = sol::state();
        lua.open_libraries(sol::lib::base, sol::lib::io, sol::lib::math, sol::lib::table);
        LuaBasicFunctions::S_RegisterTypesForLuaState(lua);
        LuaBasicFunctions::S_RisterBasicFunctionsForLuaState(lua);
    }

    LuaEntity* ScriptingEngine::FromEntityToLuaEntity(const Entity& _entity)
    {
        // get or create enitity
        auto idLuaEntity = LuaEntityMap.find(_entity.GetUID());
        if (idLuaEntity == LuaEntityMap.end())
        {
            LuaEntityMap[_entity.GetUID()] = LuaEntity();
        }

        // updating lua entity;
        LuaEntity* _luaEntity = &LuaEntityMap[_entity.GetUID()];

        _luaEntity->uid = _entity.GetUID();
        
        if(_entity.HasComponent<TagComponent>()) _luaEntity->components->tag = &_entity.GetComponent<TagComponent>();
        if(_entity.HasComponent<Transform>()) _luaEntity->components->transform = &_entity.GetComponent<Transform>();
        if(_entity.HasComponent<RigidBody2D>()) _luaEntity->components->rigid_body = &_entity.GetComponent<RigidBody2D>();
        if(_entity.HasComponent<SpriteComponent>()) _luaEntity->components->sprite = &_entity.GetComponent<SpriteComponent>();
        if(_entity.HasComponent<WorldSpaceText>()) _luaEntity->components->worldspace_text = &_entity.GetComponent<WorldSpaceText>();
        if(_entity.HasComponent<OverlayText>()) _luaEntity->components->overlay_text = &_entity.GetComponent<OverlayText>();
        
        return _luaEntity;
    }

    void ScriptingEngine::InitGameLoop()
    {
        updatedScripts.clear();
        beginCollisionUpdate.clear();
        endCollisionUpdate.clear();
        LuaEntityMap.clear();
    }

    void ScriptingEngine::HandleScript(const std::string& filename, const Entity& entity)
    {
        LoadScript(filename, entity);
        
        if (updatedScripts.find(entity.GetUID()) == updatedScripts.end())
        {
            // TODO: check if oncreate exists

            // LuaEntityMap[entity.GetUID()] = *FromEntityToLuaEntity(entity);
            ExecuteFunction("OnCreate");
            updatedScripts.insert(entity.GetUID());
        }

        if (beginCollisionUpdate.find(entity.GetUID()) != beginCollisionUpdate.end())
        {
            ALAS_CORE_INFO("{0} {1}", entity.GetComponent<TagComponent>().Tag, entity.GetUID());
            auto& luaHandle = entity.GetComponent<LuaScriptComponent>();
            if (luaHandle._beginCollisionFunctionName != LUA_SCRIPT_NO_COLLISION_FUNC)
            {
                std::set<UID> entities = beginCollisionUpdate[entity.GetUID()]; 
                for (auto it = entities.begin(); it != entities.end(); it++)
                {
                    Entity* currentEntity = Scene::GetGameLoopScene()->GetEntityByIdIfExists(*it);
                    
                    ALAS_CORE_INFO("{0}", currentEntity->GetComponent<TagComponent>().Tag, currentEntity->GetUID());
                    if (!currentEntity) continue;
                    ExecuteFunction(luaHandle._beginCollisionFunctionName, *currentEntity);
                }
            }
            beginCollisionUpdate.erase(entity.GetUID());
        }

        if (endCollisionUpdate.find(entity.GetUID()) != endCollisionUpdate.end())
        {
            auto& luaHandle = entity.GetComponent<LuaScriptComponent>();
            if (luaHandle._endCollisionFunctionName != LUA_SCRIPT_NO_COLLISION_FUNC)
            {
                std::set<UID> entities = endCollisionUpdate[entity.GetUID()]; 
                for (auto it = entities.begin(); it != entities.end(); it++)
                {
                    Entity* currentEntity = Scene::GetGameLoopScene()->GetEntityByIdIfExists(*it);
                    if (!currentEntity) continue;
                    ExecuteFunction(luaHandle._endCollisionFunctionName, *currentEntity);
                }
            }
            endCollisionUpdate.erase(entity.GetUID());
        }
        
        ExecuteFunction("Update");


    }

    void ScriptingEngine::LoadScript(const std::string& filename, const Entity& entity)
    {
        ALAS_PROFILE_FUNCTION()
        try
        {
            LuaBasicFunctions::S_RegisterEntityRelatedFunctions(lua, entity);
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

    void ScriptingEngine::ExecuteFunction(const std::string& functionName, const Entity& entity)
    {
        FromEntityToLuaEntity(entity);
        ALAS_PROFILE_FUNCTION()
        auto result = lua[functionName](
            LuaEntityMap[entity.GetUID()]
        );

        if (!result.valid())
        {
            sol::error e = result;
            ALAS_CORE_ERROR("{0}", e.what());
        }
    }

    void ScriptingEngine::AddBeginCollisionUpdate(UID first, UID second)
    {
        ALAS_PROFILE_FUNCTION();
        beginCollisionUpdate[first].insert(second);
    }
    
    void ScriptingEngine::AddEndCollisionUpdate(UID first, UID second)
    {
        ALAS_PROFILE_FUNCTION();
        endCollisionUpdate[first].insert(second);
    }
}