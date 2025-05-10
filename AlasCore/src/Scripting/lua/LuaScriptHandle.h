#pragma once


namespace Alas
{
    class Entity;
    struct LuaEntity;

    class LuaScriptHandle
    {
        friend class LuaScriptComponent;

    public:
        LuaScriptHandle() = default;
        LuaScriptHandle(const LuaScriptHandle& other) = default;
        LuaScriptHandle(Shared<Entity> entity);

        // void DestroyEntity

        Shared<LuaEntity> GetSelf();

        void BindBeginCollisionFunction(const char* functionName);
        void UnbindBeginCollisionFunction();

        void BindEndCollisionFunction(const char* functionName);
        void UnbindEndCollisionFunction();

        // void DeleteEntity
    private:
        void UpdateLuaEntity();

    private:
        Shared<Entity> _entity;
        Shared<LuaEntity> _luaEntity;
    };
} // namespace Alas
