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

        LuaComponents()
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

        LuaEntity()
        {
            components = new LuaComponents();
        }
    };
} // namespace Alas
