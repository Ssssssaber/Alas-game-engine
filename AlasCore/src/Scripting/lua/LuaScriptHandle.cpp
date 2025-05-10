#include "LuaScriptHandle.h"
#include "Entity/Entity.h"
#include "LuaComponents.h"
namespace Alas
{
    LuaScriptHandle::LuaScriptHandle(Shared<Entity> entity) : _entity(entity)
    {
        _luaEntity.reset(new LuaEntity());
        UpdateLuaEntity();
    }

    Shared<LuaEntity> LuaScriptHandle::GetSelf()
    {
        UpdateLuaEntity();
        return _luaEntity;
    }

    void LuaScriptHandle::UpdateLuaEntity()
    {
        _luaEntity->uid = _entity->GetUID();
        _luaEntity->tag = _entity->HasComponent<TagComponent>() ? &_entity->GetComponent<TagComponent>() : nullptr;
        _luaEntity->transform = _entity->HasComponent<Transform>() ? &_entity->GetComponent<Transform>() : nullptr;
        _luaEntity->rigid_body = _entity->HasComponent<RigidBody2D>() ? &_entity->GetComponent<RigidBody2D>() : nullptr;
        _luaEntity->sprite = _entity->HasComponent<SpriteComponent>() ? &_entity->GetComponent<SpriteComponent>() : nullptr;
        _luaEntity->worldspace_text = _entity->HasComponent<WorldSpaceText>() ? &_entity->GetComponent<WorldSpaceText>() : nullptr;
        _luaEntity->overlay_text = _entity->HasComponent<OverlayText>() ? &_entity->GetComponent<OverlayText>() : nullptr;
    }

    void LuaScriptHandle::BindBeginCollisionFunction(const char* functionName)
    {
        if (!_entity->HasComponent<LuaScriptComponent>()) return; // could be unnecessary
        
        auto& luaHandle = _entity->GetComponent<LuaScriptComponent>();
        luaHandle._beginCollisionFunctionName = functionName;
        // luaHandle._endCollisionFunctionName;
    }

    void LuaScriptHandle::UnbindBeginCollisionFunction()
    {
        if (!_entity->HasComponent<LuaScriptComponent>()) return; // could be unnecessary
        
        auto& luaHandle = _entity->GetComponent<LuaScriptComponent>();
        luaHandle._beginCollisionFunctionName = LUA_SCRIPT_NO_COLLISION_FUNC;
        // luaHandle._endCollisionFunctionName;
    }

    void LuaScriptHandle::BindEndCollisionFunction(const char* functionName)
    {
        if (!_entity->HasComponent<LuaScriptComponent>()) return; // could be unnecessary
        
        auto& luaHandle = _entity->GetComponent<LuaScriptComponent>();
        luaHandle._endCollisionFunctionName = functionName;
    }

    void LuaScriptHandle::UnbindEndCollisionFunction()
    {
        if (!_entity->HasComponent<LuaScriptComponent>()) return; // could be unnecessary
        
        auto& luaHandle = _entity->GetComponent<LuaScriptComponent>();
        luaHandle._endCollisionFunctionName = LUA_SCRIPT_NO_COLLISION_FUNC;
        // luaHandle._endCollisionFunctionName;
    }
} // namespace Alas
