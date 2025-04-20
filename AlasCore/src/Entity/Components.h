#pragma once

#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Texture.h"
#include "Scripting/lua/LuaScriptHandle.h"


#include <glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/quaternion.hpp>
#include <string>

#define ID_C "ID"
#define TAG_C "Tag"

#define TRANSFORM_C "Transform"
#define TRANSFORM_C_POSITION "Position"
#define TRANSFORM_C_ROTATION "Rotation"
#define TRANSFORM_C_SCLAE "Scale"

#define SPRITE_C "Sprite"
#define SPRITE_C_SHADER "Shader"
#define SPRITE_C_TEXTURE "Texture"
#define SPRITE_C_COLOR "Color"

#define RIGID_BODY_2D_C "RigidBody2D"
#define RIGID_BODY_2D_C_TYPE "Type"
#define RIGID_BODY_2D_C_MASS "Mass"
#define RIGID_BODY_2D_C_GRAVITY_SCALE "Gravity scale"

#define BOX_COLLIDER_2D_C "Box Collider"
#define BOX_COLLIDER_2D_C_OFFSET "Offset"
#define BOX_COLLIDER_2D_C_SIZE "Size"

// for parsing yaml rigid body type
#define DEFAULT_RIGID_BODY_2D_TYPE RigidBody2D::BodyType::Dynamic
#define RIGID_BODY_2D_TYPE_DYNAMIC_STR "Dynamic"
#define RIGID_BODY_2D_TYPE_KINEMATIC_STR "Kinematic"
#define RIGID_BODY_2D_TYPE_STATIC_STR "Static"

#define LUA_SCRIPT_C "Lua script"
#define LUA_SCRIPT_C_FILE "Lua script file"

#define OVERLAY_TEXT_C "Overlay Text"
#define OVERLAY_TEXT_C_SCREEN_POSITION "Screen Position"
#define OVERLAY_TEXT_C_ROTATION "Rotation"
#define OVERLAY_TEXT_C_SCALE "Scale"
#define OVERLAY_TEXT_C_DISPLAY_TEXT "Display Text"
#define OVERLAY_TEXT_C_COLOR "Color"

#define WORLD_SPACE_TEXT_C "World Space Text"
#define WORLD_SPACE_TEXT_C_OFFSET "Position"
#define WORLD_SPACE_TEXT_C_ROTATION "Rotation"
#define WORLD_SPACE_TEXT_C_SCALE "Scale"
#define WORLD_SPACE_TEXT_C_DISPLAY_TEXT "Display Text"
#define WORLD_SPACE_TEXT_C_COLOR "Color"

namespace Alas
{
    struct IDComponent
	{
		UID ID;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
	};

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
	};

	struct Transform
	{
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };
        
        glm::mat4 _modelMatrix;

		Transform() = default;
		Transform(const Transform&) = default;
		Transform(const glm::vec3& translation)
			: Position(translation) {}

        glm::mat4 CalculateModelMatrix()
        {
            return glm::translate(glm::mat4(1.0f), Position) *
                glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) *
                glm::scale(glm::mat4(1.0f), Scale);
        }
	};

    // Forward declaration
	class ScriptableEntity;

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity*(*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};

    struct LuaScriptComponent
    {   
        std::string Filepath;
        LuaScriptHandle Handle;

        LuaScriptComponent() = default;
		LuaScriptComponent(const LuaScriptComponent&) = default;
		LuaScriptComponent(const std::string& filepath)
			: Filepath(filepath) {}
    };

    struct SpriteComponent
    {
        Shared<Shader> c_Shader;
        Shared<Texture> c_Texture;
        glm::vec4 Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

        SpriteComponent() = default;
		SpriteComponent(const SpriteComponent&) = default;
        SpriteComponent(Shared<Texture> texture, Shared<Shader> shader) :
            c_Texture(texture), c_Shader(shader) {}
    };

    struct RigidBody2D
    {
        enum class BodyType { Static = 0, Kinematic = 1, Dynamic = 2};
        
        BodyType Type = BodyType::Dynamic;
        glm::vec2 Velocity = glm::vec2(0.0f);
        
        float Mass = 1.0;

        float GravityScale = 1.0;


        RigidBody2D() = default;
		RigidBody2D(const RigidBody2D&) = default;
        RigidBody2D(RigidBody2D::BodyType bodyType) :
            Type(bodyType) {}

        static std::string TypeToString(BodyType type)
        {
            switch (type)
            {
                case (BodyType::Dynamic): return RIGID_BODY_2D_TYPE_DYNAMIC_STR;            
                case (BodyType::Kinematic): return RIGID_BODY_2D_TYPE_KINEMATIC_STR;
                case (BodyType::Static): return RIGID_BODY_2D_TYPE_STATIC_STR;
            }
            return "None";
        }
    
        static BodyType StringToType(const std::string& string)
        {
            if (string == RIGID_BODY_2D_TYPE_DYNAMIC_STR) return BodyType::Dynamic;
            else if (string == RIGID_BODY_2D_TYPE_KINEMATIC_STR) return BodyType::Kinematic;
            else if (string == RIGID_BODY_2D_TYPE_STATIC_STR) return BodyType::Static;
            else
            {
                ALAS_CORE_ERROR(
                    "Unknown RigidBody2D BodyType: {0}. Setting default type ({0}).",
                    string,
                    TypeToString(DEFAULT_RIGID_BODY_2D_TYPE));
                return DEFAULT_RIGID_BODY_2D_TYPE;
            }
        }
        
    };

    struct BoxCollider2D
    {
        glm::vec2 Offset = {0.0f, 0.0f};
        glm::vec2 Size = {250.0f, 250.0f};
        
        BoxCollider2D() = default;
		BoxCollider2D(const BoxCollider2D&) = default;
        BoxCollider2D(glm::vec2 offset, glm::vec2 size) :
            Offset(offset), Size(size) {}
        
    };

    struct WorldSpaceText
    {
        glm::vec2 Offset = {0.0f, 0.0f};
        float Rotation = 0.0f;
        glm::vec2 Scale = {1.0f, 1.0f};
        std::string DisplayText;
        glm::vec4 Color = glm::vec4{1.0f};

        WorldSpaceText() = default;
		WorldSpaceText(const WorldSpaceText&) = default;
        WorldSpaceText(const std::string& text) :
            DisplayText(text) {}
        WorldSpaceText(const std::string& text, const glm::vec4& color) :
            DisplayText(text), Color(color) {}

    };

    struct OverlayText
    {
        glm::vec2 ScreenPosition = {0.0f, 0.0f};
        float Rotation = 0.0f;
        glm::vec2 Scale = {1.0f, 1.0f};
        std::string DisplayText;
        glm::vec4 Color = glm::vec4{1.0f};

        OverlayText() = default;
		OverlayText(const OverlayText&) = default;
        OverlayText(const std::string& text) :
            DisplayText(text) {}
        OverlayText(const std::string& text, const glm::vec4& color) :
            DisplayText(text), Color(color) {}

    };
} // namespace Alas
