#include "LuaScriptHandle.h"
#include "Entity/Entity.h"

namespace Alas
{
    LuaScriptHandle::LuaScriptHandle(Shared<Entity> entity) : _entity(entity) {}

    vec2* LuaScriptHandle::GetVelocity()
    {
        if (!_entity->HasComponent<RigidBody2D>()) 
        {
            return nullptr;
        }
        
        auto velocity = _entity->GetComponent<RigidBody2D>().Velocity;

        return new vec2{ velocity.x, velocity.y };
    }

    void LuaScriptHandle::SetVelocity(vec2* velocity)
    {
        if (!_entity->HasComponent<RigidBody2D>()) return;
        
        auto& rigid = _entity->GetComponent<RigidBody2D>();
        rigid.Velocity = glm::vec2(velocity->x, velocity->y);
    }

    transform* LuaScriptHandle::GetTransform()
    {
        if (!_entity->HasComponent<Transform>()) 
        {
            return nullptr;
        }

        auto& transformLua = _entity->GetComponent<Transform>();
        return new transform{
            {transformLua.Position.x, transformLua.Position.y},
            transformLua.Rotation.z,
            {transformLua.Scale.x, transformLua.Scale.y}
        }; 
               
    }

    void LuaScriptHandle::SetTransform(transform* new_transform)
    {
        if (!_entity->HasComponent<Transform>()) return;
        
        auto& currentTransform = _entity->GetComponent<Transform>();
        currentTransform.Position = glm::vec3(new_transform->position.x, new_transform->position.y, 0.0f);
        currentTransform.Rotation.z = new_transform->rotation;
        currentTransform.Scale = glm::vec3(new_transform->scale.x, new_transform->scale.y, 0.0f);
    }

    rigid_body* LuaScriptHandle::GetRigidBody()
    {
        if (!_entity->HasComponent<RigidBody2D>()) 
        {
            return nullptr;
        }

        auto& rigid = _entity->GetComponent<RigidBody2D>();
        return new rigid_body{
            body_type(rigid.Type),
            {rigid.Velocity.x, rigid.Velocity.y},
            rigid.Mass    
        };   
    }


    void LuaScriptHandle::SetRigidBody(rigid_body* new_rigid_body)
    {
        if (!_entity->HasComponent<RigidBody2D>()) return;
        
        auto& rigid = _entity->GetComponent<RigidBody2D>();

        rigid.Type = RigidBody2D::BodyType(new_rigid_body->type);
        rigid.Velocity = glm::vec2(new_rigid_body->velocity.x, new_rigid_body->velocity.y);    
        rigid.Mass = new_rigid_body->mass;
    }

    sprite* LuaScriptHandle::GetSprite()
    {
        if (!_entity->HasComponent<SpriteComponent>()) 
        {
            return nullptr;
        }

        auto& alas_sprite = _entity->GetComponent<SpriteComponent>();
        return new sprite{ {alas_sprite.Color.x, alas_sprite.Color.y, alas_sprite.Color.z, alas_sprite.Color.w} };   
    }

    void LuaScriptHandle::SetSprite(sprite* new_sprite)
    {
        if (!_entity->HasComponent<SpriteComponent>()) return;
        
        auto& sprite = _entity->GetComponent<SpriteComponent>();
        sprite.Color = glm::vec4(new_sprite->color.x, new_sprite->color.y, new_sprite->color.z, new_sprite->color.w);
    }
} // namespace Alas
