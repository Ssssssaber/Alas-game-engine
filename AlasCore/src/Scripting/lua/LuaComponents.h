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
} // namespace Alas
