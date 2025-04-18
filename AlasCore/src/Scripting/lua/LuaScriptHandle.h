#pragma once

#include "LuaComponents.h"

namespace Alas
{
    class Entity;

    class LuaScriptHandle
    {
    public:
        LuaScriptHandle() = default;
        LuaScriptHandle(const LuaScriptHandle& other) = default;
        LuaScriptHandle(Shared<Entity> entity);
        
        vec2* GetVelocity();
        void SetVelocity(vec2* new_velocity);

        transform* GetTransform();
        void SetTransform(transform* new_transform);

        rigid_body* GetRigidBody();
        void SetRigidBody(rigid_body* new_rigid_body);

        sprite* GetSprite();
        void SetSprite(sprite* new_sprite);
    
    private:
        Shared<Entity> _entity;
    };
} // namespace Alas
