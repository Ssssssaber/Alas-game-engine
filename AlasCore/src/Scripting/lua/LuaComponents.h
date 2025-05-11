#include "Entity/Components.h"
namespace Alas
{
    struct LuaComponents
    {
        TagComponent* tag;
        Transform* transform;
        RigidBody2D* rigid_body;
        BoxCollider2D* box_collider;
        SpriteComponent* sprite;
        WorldSpaceText* worldspace_text;
        OverlayText* overlay_text;

        LuaComponents::LuaComponents()
        {
            tag = new TagComponent();
            transform = new Transform();
            rigid_body = new RigidBody2D();
            box_collider = new BoxCollider2D();
            sprite = new SpriteComponent();
            worldspace_text = new WorldSpaceText();
            overlay_text = new OverlayText();
        }
    };

    struct LuaEntity
    {
        uint32_t uid;
        LuaComponents* components;

        LuaEntity::LuaEntity()
        {
            components = new LuaComponents();
        }
    };

    static LuaEntity* FromEntityToLuaEntity(const Entity& _entity)
    {
        LuaEntity* _luaEntity = new LuaEntity();
        // _luaEntity.reset();

        _luaEntity->uid = _entity.GetUID();
        // _luaEntity->components->tag = _entity.HasComponent<TagComponent>() ? &_entity.GetComponent<TagComponent>() : nullptr;
        // _luaEntity->components->transform = _entity.HasComponent<Transform>() ? &_entity.GetComponent<Transform>() : nullptr;
        // _luaEntity->components->rigid_body = _entity.HasComponent<RigidBody2D>() ? &_entity.GetComponent<RigidBody2D>() : nullptr;
        // _luaEntity->components->sprite = _entity.HasComponent<SpriteComponent>() ? &_entity.GetComponent<SpriteComponent>() : nullptr;
        // _luaEntity->components->worldspace_text = _entity.HasComponent<WorldSpaceText>() ? &_entity.GetComponent<WorldSpaceText>() : nullptr;
        // _luaEntity->components->overlay_text = _entity.HasComponent<OverlayText>() ? &_entity.GetComponent<OverlayText>() : nullptr;

        if(_entity.HasComponent<TagComponent>()) _luaEntity->components->tag = &_entity.GetComponent<TagComponent>();
        if(_entity.HasComponent<Transform>()) _luaEntity->components->transform = &_entity.GetComponent<Transform>();
        if(_entity.HasComponent<RigidBody2D>()) _luaEntity->components->rigid_body = &_entity.GetComponent<RigidBody2D>();
        if(_entity.HasComponent<SpriteComponent>()) _luaEntity->components->sprite = &_entity.GetComponent<SpriteComponent>();
        if(_entity.HasComponent<WorldSpaceText>()) _luaEntity->components->worldspace_text = &_entity.GetComponent<WorldSpaceText>();
        if(_entity.HasComponent<OverlayText>()) _luaEntity->components->overlay_text = &_entity.GetComponent<OverlayText>();

        return _luaEntity;
    }
} // namespace Alas
