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

    worldspace_text* LuaScriptHandle::GetWorldspaceText()
    {
        if (!_entity->HasComponent<WorldSpaceText>()) 
        {
            return nullptr;
        }

        auto& worldSpaceText = _entity->GetComponent<WorldSpaceText>();
        return new worldspace_text{ 
            {worldSpaceText.Offset.x, worldSpaceText.Offset.y},
            worldSpaceText.Rotation,
            {worldSpaceText.Scale.x, worldSpaceText.Scale.y},
            worldSpaceText.DisplayText,
            {worldSpaceText.Color.x, worldSpaceText.Color.y, worldSpaceText.Color.z, worldSpaceText.Color.w},
        };
    }

    void LuaScriptHandle::SetWorldspaceText(worldspace_text* new_text)
    {
        if (!_entity->HasComponent<WorldSpaceText>()) return;
        
        auto& worldSpaceText = _entity->GetComponent<WorldSpaceText>();
        worldSpaceText.Offset = {new_text->offset.x, new_text->offset.y};
        worldSpaceText.Rotation = new_text->rotation;
        worldSpaceText.Scale = {new_text->scale.x, new_text->scale.y};
        worldSpaceText.DisplayText = new_text->display_text;
        worldSpaceText.Color = {new_text->color.x, new_text->color.y, new_text->color.z, new_text->color.w};
    }

    overlay_text* LuaScriptHandle::GetOverlayText()
    {
        if (!_entity->HasComponent<OverlayText>()) 
        {
            return nullptr;
        }

        auto& overlayText = _entity->GetComponent<OverlayText>();
        return new overlay_text{ 
            {overlayText.ScreenPosition.x, overlayText.ScreenPosition.y},
            overlayText.Rotation,
            {overlayText.Scale.x, overlayText.Scale.y},
            overlayText.DisplayText,
            {overlayText.Color.x, overlayText.Color.y, overlayText.Color.z, overlayText.Color.w},
        };
    }

    void LuaScriptHandle::SetOverlayText(overlay_text* new_text)
    {
        if (!_entity->HasComponent<OverlayText>()) return;
        
        auto& overlayText = _entity->GetComponent<OverlayText>();
        overlayText.ScreenPosition = {new_text->screen_position.x, new_text->screen_position.y};
        overlayText.Rotation = new_text->rotation;
        overlayText.Scale = {new_text->scale.x, new_text->scale.y};
        overlayText.DisplayText = new_text->display_text;
        overlayText.Color = {new_text->color.x, new_text->color.y, new_text->color.z, new_text->color.w};
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
