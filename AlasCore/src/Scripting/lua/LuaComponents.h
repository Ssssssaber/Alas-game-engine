#pragma once

namespace Alas
{
    enum class body_type 
    { 
        STATIC = 0,
        KINEMATIC = 1,
        DYNAMIC = 2
    };

    struct vec2
    {
        float x;
        float y;
    };

    struct transform
    {
        vec2 position;
        float rotation;
        vec2 scale;
    };

    struct rigid_body
    {
        body_type type;
        vec2 velocity;
        float mass;
    };

    struct vec3
    {
        float x;
        float y;
        float z;
    };

    struct vec4
    {
        float x;
        float y;
        float z;
        float w;
    };

    struct sprite
    {
        vec4 color;
    };

    struct worldspace_text
    {
        vec2 offset;
        float rotation;
        vec2 scale;
        std::string display_text;
        vec4 color;
    };
    struct overlay_text
    {
        vec2 screen_position;
        float rotation;
        vec2 scale;
        std::string display_text;
        vec4 color;
    };
} // namespace Alas
