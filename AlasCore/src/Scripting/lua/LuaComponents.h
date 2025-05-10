#include "Entity/Components.h"
namespace Alas
{
    struct LuaEntity
    {
        uint32_t uid;
        TagComponent* tag;
        Transform* transform;
        RigidBody2D* rigid_body;
        SpriteComponent* sprite;
        WorldSpaceText* worldspace_text;
        OverlayText* overlay_text;
    };

    static Shared<LuaEntity> FromEntityToLuaEntity(const Entity& _entity)
    {
        Shared<LuaEntity> _luaEntity;
        _luaEntity.reset(new LuaEntity);

        _luaEntity->uid = _entity.GetUID();
        _luaEntity->tag = _entity.HasComponent<TagComponent>() ? &_entity.GetComponent<TagComponent>() : nullptr;
        _luaEntity->transform = _entity.HasComponent<Transform>() ? &_entity.GetComponent<Transform>() : nullptr;
        _luaEntity->rigid_body = _entity.HasComponent<RigidBody2D>() ? &_entity.GetComponent<RigidBody2D>() : nullptr;
        _luaEntity->sprite = _entity.HasComponent<SpriteComponent>() ? &_entity.GetComponent<SpriteComponent>() : nullptr;
        _luaEntity->worldspace_text = _entity.HasComponent<WorldSpaceText>() ? &_entity.GetComponent<WorldSpaceText>() : nullptr;
        _luaEntity->overlay_text = _entity.HasComponent<OverlayText>() ? &_entity.GetComponent<OverlayText>() : nullptr;

        return _luaEntity;
    }
} // namespace Alas
