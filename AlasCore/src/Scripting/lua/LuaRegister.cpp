#include "LuaRegister.h"

#include "Entity/Scene.h"
#include "ScriptingEngine.h"
namespace Alas
{
    float LuaBasicFunctions::GetDeltaTime()
    {
        return Time::getDeltaTime();
    }

    float LuaBasicFunctions::GetElapsedTime()
    {
        return Time::GetTimeInSeconds();
    }

    bool LuaBasicFunctions::IsButtonPressed(ALAS_Scancode keyCode)
    {
        return Input::IsKeyPressed(keyCode);
    }

    glm::vec2 LuaBasicFunctions::Normalize2(glm::vec2 vec)
    {
        if (vec.length > 0)
            glm::normalize(vec);
        return vec;
    }
    
    glm::vec3 LuaBasicFunctions::Normalize3(glm::vec3 vec)
    {
        if (vec.length > 0)
            glm::normalize(vec);
        return vec;
    }
    
    glm::vec4 LuaBasicFunctions::Normalize4(glm::vec4 vec)
    {
        if (vec.length > 0)
            glm::normalize(vec);
        return vec;
    }

    void LuaBasicFunctions::DestroyEntity(LuaEntity* entity)
    {
        Scene::GetGameLoopScene()->DeleteEntityWithId(entity->uid);
    }

    LuaEntity* LuaBasicFunctions::GetEntityWithTag(const std::string& tag)
    {
        Entity* toCopy = Scene::GetGameLoopScene()->FindEntityWithTag(tag);
        return ScriptingEngine::FromEntityToLuaEntity(*toCopy);
    }
    
    LuaEntity* LuaBasicFunctions::CreateEntityWithComponents(LuaComponents* components)
    {
        if (!components->tag) return nullptr;

        Entity entity = Scene::GetGameLoopScene()->CreateEntity(components->tag->Tag);

        entity.GetComponent<TagComponent>().Tag = components->tag->Tag;
        if (components->transform)
        {
            auto& transform = entity.GetComponent<Transform>();
            transform.Position = components->transform->Position;
            transform.Rotation = components->transform->Rotation;
            transform.Scale = components->transform->Scale;
        }

        if (components->sprite)
        {
            auto& sprite = entity.AddComponent<SpriteComponent>();
            sprite.c_Shader = components->sprite->c_Shader;
            sprite.c_Texture = components->sprite->c_Texture;
            sprite.Color = components->sprite->Color;
        }

        if (components->rigid_body)
        {
            auto& rigid_body = entity.AddComponent<RigidBody2D>();
            rigid_body.Velocity = components->rigid_body->Velocity;
            rigid_body.GravityScale = components->rigid_body->GravityScale;
            rigid_body.Mass = components->rigid_body->Mass;

            if (components->box_collider)
            {
                auto& box_collider = entity.AddComponent<BoxCollider2D>();
                box_collider.Offset = components->box_collider->Offset;
                box_collider.Size = components->box_collider->Size;
            }

            Scene::GetGameLoopScene()->AddPhysicsBody(entity);
        }

        return ScriptingEngine::FromEntityToLuaEntity(entity);
    }

    void LuaBasicFunctions::S_RegisterEntityRelatedFunctions(sol::state& lua, Entity entity)
    {
        lua.set_function("BindBeginCollisionFunction", &LuaScriptHandle::BindBeginCollisionFunction, &entity.GetComponent<LuaScriptComponent>().Handle);
        lua.set_function("UnbindBeginCollisionFunction", &LuaScriptHandle::UnbindBeginCollisionFunction, &entity.GetComponent<LuaScriptComponent>().Handle);

        lua.set_function("BindEndCollisionFunction", &LuaScriptHandle::BindEndCollisionFunction, &entity.GetComponent<LuaScriptComponent>().Handle);
        lua.set_function("UnbindEndCollisionFunction", &LuaScriptHandle::UnbindEndCollisionFunction, &entity.GetComponent<LuaScriptComponent>().Handle);

        lua.set_function("GetSelf", &LuaScriptHandle::GetSelf, &entity.GetComponent<LuaScriptComponent>().Handle);
    }

    void LuaBasicFunctions::S_RisterBasicFunctionsForLuaState(sol::state& luaState)
    {
        luaState.set_function("GetDeltaTime", &LuaBasicFunctions::GetDeltaTime);
        luaState.set_function("GetElapsedTime", &LuaBasicFunctions::GetElapsedTime);
        luaState.set_function("IsButtonPressed", &LuaBasicFunctions::IsButtonPressed);

        luaState.set_function("Normalize2", &LuaBasicFunctions::Normalize2);
        luaState.set_function("Normalize3", &LuaBasicFunctions::Normalize3);
        luaState.set_function("Normalize4", &LuaBasicFunctions::Normalize4);

        luaState.set_function("DestroyEntity", &LuaBasicFunctions::DestroyEntity);

        luaState.set_function("CreateEntityWithComponents", &LuaBasicFunctions::CreateEntityWithComponents);
        luaState.set_function("GetEntityWithTag", &LuaBasicFunctions::GetEntityWithTag);
    }

    void LuaBasicFunctions::S_RegisterTypesForLuaState(sol::state& luaState)
    {
        luaState.new_usertype<glm::vec2>("vec2",
            sol::constructors<glm::vec2()>(),
            "x", &glm::vec2::x,
            "y", &glm::vec2::y
        );

        luaState.new_usertype<glm::vec3>("vec3",
            sol::constructors<glm::vec3()>(),
            "x", &glm::vec3::x,
            "y", &glm::vec3::y,
            "z", &glm::vec3::z
        );

        luaState.new_usertype<glm::vec4>("vec4",
            sol::constructors<glm::vec4()>(),
            "x", &glm::vec4::x,
            "y", &glm::vec4::y,
            "z", &glm::vec4::z,
            "w", &glm::vec4::w
        );

        luaState.new_usertype<TagComponent>("tag",
            sol::constructors<TagComponent()>(),
            "text", &TagComponent::Tag
        );
    
        luaState.new_usertype<Transform>("transform",
            sol::constructors<Transform()>(),
            "position", &Transform::Position,
            "rotation", &Transform::Rotation,
            "scale", &Transform::Scale
        );

        luaState.new_enum<RigidBody2D::BodyType>("body_type", {
                {"STATIC", RigidBody2D::BodyType::Static},
                {"KINEMATIC", RigidBody2D::BodyType::Kinematic},
                {"DYNAMIC", RigidBody2D::BodyType::Dynamic}
            } 
        );
    
        luaState.new_usertype<RigidBody2D>("rigid_body",
            sol::constructors<RigidBody2D()>(),
            "type", &RigidBody2D::Type,
            "velocity", &RigidBody2D::Velocity,
            "mass", &RigidBody2D::Mass
        );

        luaState.new_usertype<SpriteComponent>("sprite",
            sol::constructors<SpriteComponent()>(),
            "color", &SpriteComponent::Color
        );

        luaState.new_usertype<WorldSpaceText>("worldspace_text",
            sol::constructors<WorldSpaceText()>(),
            "offset", &WorldSpaceText::Offset,
            "rotation", &WorldSpaceText::Rotation,
            "scale", &WorldSpaceText::Scale,
            "display_text", &WorldSpaceText::DisplayText,
            "color", &WorldSpaceText::Color
        );

        luaState.new_usertype<OverlayText>("overlay_text",
            sol::constructors<OverlayText()>(),
            "screen_position", &OverlayText::ScreenPosition,
            "rotation", &OverlayText::Rotation,
            "scale", &OverlayText::Scale,
            "display_text", &OverlayText::DisplayText,
            "color", &OverlayText::Color
        );

        luaState.new_usertype<LuaComponents>("components",
            sol::constructors<LuaComponents()>(),
            "tag", &LuaComponents::tag,
            "transform", &LuaComponents::transform,
            "rigid_body", &LuaComponents::rigid_body,
            "box_collider", &LuaComponents::box_collider,
            "sprite", &LuaComponents::sprite,
            "worldspace_text", &LuaComponents::worldspace_text,
            "overlay_text", &LuaComponents::overlay_text
        );

        luaState.new_usertype<LuaEntity>("entity",
            sol::constructors<LuaEntity()>(),
            "uid", &LuaEntity::uid,
            "components", &LuaEntity::components
        );

    
        luaState.new_enum<ALAS_Scancode>("KeyCode", {
            {"KEY_UNKNOWN", ALAS_KEY_UNKNOWN},
            {"KEY_A", ALAS_KEY_A},
            {"KEY_B", ALAS_KEY_B},
            {"KEY_C", ALAS_KEY_C},
            {"KEY_D", ALAS_KEY_D},
            {"KEY_E", ALAS_KEY_E},
            {"KEY_F", ALAS_KEY_F},
            {"KEY_G", ALAS_KEY_G},
            {"KEY_H", ALAS_KEY_H},
            {"KEY_I", ALAS_KEY_I},
            {"KEY_J", ALAS_KEY_J},
            {"KEY_K", ALAS_KEY_K},
            {"KEY_L", ALAS_KEY_L},
            {"KEY_M", ALAS_KEY_M},
            {"KEY_N", ALAS_KEY_N},
            {"KEY_O", ALAS_KEY_O},
            {"KEY_P", ALAS_KEY_P},
            {"KEY_Q", ALAS_KEY_Q},
            {"KEY_R", ALAS_KEY_R},
            {"KEY_S", ALAS_KEY_S},
            {"KEY_T", ALAS_KEY_T},
            {"KEY_U", ALAS_KEY_U},
            {"KEY_V", ALAS_KEY_V},
            {"KEY_W", ALAS_KEY_W},
            {"KEY_X", ALAS_KEY_X},
            {"KEY_Y", ALAS_KEY_Y},
            {"KEY_Z", ALAS_KEY_Z},
            {"KEY_1", ALAS_KEY_1},
            {"KEY_2", ALAS_KEY_2},
            {"KEY_3", ALAS_KEY_3},
            {"KEY_4", ALAS_KEY_4},
            {"KEY_5", ALAS_KEY_5},
            {"KEY_6", ALAS_KEY_6},
            {"KEY_7", ALAS_KEY_7},
            {"KEY_8", ALAS_KEY_8},
            {"KEY_9", ALAS_KEY_9},
            {"KEY_0", ALAS_KEY_0},
            {"KEY_RETURN", ALAS_KEY_RETURN},
            {"KEY_ESCAPE", ALAS_KEY_ESCAPE},
            {"KEY_BACKSPACE", ALAS_KEY_BACKSPACE},
            {"KEY_TAB", ALAS_KEY_TAB},
            {"KEY_SPACE", ALAS_KEY_SPACE},
            {"KEY_MINUS", ALAS_KEY_MINUS},
            {"KEY_EQUALS", ALAS_KEY_EQUALS},
            {"KEY_LEFTBRACKET", ALAS_KEY_LEFTBRACKET},
            {"KEY_RIGHTBRACKET", ALAS_KEY_RIGHTBRACKET},
            {"KEY_BACKSLASH", ALAS_KEY_BACKSLASH},
            {"KEY_NONUSHASH", ALAS_KEY_NONUSHASH},
            {"KEY_SEMICOLON", ALAS_KEY_SEMICOLON},
            {"KEY_APOSTROPHE", ALAS_KEY_APOSTROPHE},
            {"KEY_GRAVE", ALAS_KEY_GRAVE},
            {"KEY_COMMA", ALAS_KEY_COMMA},
            {"KEY_PERIOD", ALAS_KEY_PERIOD},
            {"KEY_SLASH", ALAS_KEY_SLASH},
            {"KEY_CAPSLOCK", ALAS_KEY_CAPSLOCK},
            {"KEY_F1", ALAS_KEY_F1},
            {"KEY_F2", ALAS_KEY_F2},
            {"KEY_F3", ALAS_KEY_F3},
            {"KEY_F4", ALAS_KEY_F4},
            {"KEY_F5", ALAS_KEY_F5},
            {"KEY_F6", ALAS_KEY_F6},
            {"KEY_F7", ALAS_KEY_F7},
            {"KEY_F8", ALAS_KEY_F8},
            {"KEY_F9", ALAS_KEY_F9},
            {"KEY_F10", ALAS_KEY_F10},
            {"KEY_F11", ALAS_KEY_F11},
            {"KEY_F12", ALAS_KEY_F12},
            {"KEY_PRINTSCREEN", ALAS_KEY_PRINTSCREEN},
            {"KEY_SCROLLLOCK", ALAS_KEY_SCROLLLOCK},
            {"KEY_PAUSE", ALAS_KEY_PAUSE},
            {"KEY_INSERT", ALAS_KEY_INSERT},
            {"KEY_HOME", ALAS_KEY_HOME},
            {"KEY_PAGEUP", ALAS_KEY_PAGEUP},
            {"KEY_DELETE", ALAS_KEY_DELETE},
            {"KEY_END", ALAS_KEY_END},
            {"KEY_PAGEDOWN", ALAS_KEY_PAGEDOWN},
            {"KEY_RIGHT", ALAS_KEY_RIGHT},
            {"KEY_LEFT", ALAS_KEY_LEFT},
            {"KEY_DOWN", ALAS_KEY_DOWN},
            {"KEY_UP", ALAS_KEY_UP},
            {"KEY_NUMLOCKCLEAR", ALAS_KEY_NUMLOCKCLEAR},
            {"KEY_KP_DIVIDE", ALAS_KEY_KP_DIVIDE},
            {"KEY_KP_MULTIPLY", ALAS_KEY_KP_MULTIPLY},
            {"KEY_KP_MINUS", ALAS_KEY_KP_MINUS},
            {"KEY_KP_PLUS", ALAS_KEY_KP_PLUS},
            {"KEY_KP_ENTER", ALAS_KEY_KP_ENTER},
            {"KEY_KP_1", ALAS_KEY_KP_1},
            {"KEY_KP_2", ALAS_KEY_KP_2},
            {"KEY_KP_3", ALAS_KEY_KP_3},
            {"KEY_KP_4", ALAS_KEY_KP_4},
            {"KEY_KP_5", ALAS_KEY_KP_5},
            {"KEY_KP_6", ALAS_KEY_KP_6},
            {"KEY_KP_7", ALAS_KEY_KP_7},
            {"KEY_KP_8", ALAS_KEY_KP_8},
            {"KEY_KP_9", ALAS_KEY_KP_9},
            {"KEY_KP_0", ALAS_KEY_KP_0},
            {"KEY_KP_PERIOD", ALAS_KEY_KP_PERIOD},
            {"KEY_NONUSBACKSLASH", ALAS_KEY_NONUSBACKSLASH},
            {"KEY_APPLICATION", ALAS_KEY_APPLICATION},
            {"KEY_POWER", ALAS_KEY_POWER},
            {"KEY_KP_EQUALS", ALAS_KEY_KP_EQUALS},
            {"KEY_F13", ALAS_KEY_F13},
            {"KEY_F14", ALAS_KEY_F14},
            {"KEY_F15", ALAS_KEY_F15},
            {"KEY_F16", ALAS_KEY_F16},
            {"KEY_F17", ALAS_KEY_F17},
            {"KEY_F18", ALAS_KEY_F18},
            {"KEY_F19", ALAS_KEY_F19},
            {"KEY_F20", ALAS_KEY_F20},
            {"KEY_F21", ALAS_KEY_F21},
            {"KEY_F22", ALAS_KEY_F22},
            {"KEY_F23", ALAS_KEY_F23},
            {"KEY_F24", ALAS_KEY_F24},
            {"KEY_EXECUTE", ALAS_KEY_EXECUTE},
            {"KEY_HELP", ALAS_KEY_HELP},    /**< AL Integrated Help Center */
            {"KEY_MENU", ALAS_KEY_MENU},    /**< Menu (show menu) */
            {"KEY_SELECT", ALAS_KEY_SELECT},
            {"KEY_STOP", ALAS_KEY_STOP},    /**< AC Stop */
            {"KEY_AGAIN", ALAS_KEY_AGAIN},   /**< AC Redo/Repeat */
            {"KEY_UNDO", ALAS_KEY_UNDO},    /**< AC Undo */
            {"KEY_CUT", ALAS_KEY_CUT},     /**< AC Cut */
            {"KEY_COPY", ALAS_KEY_COPY},    /**< AC Copy */
            {"KEY_PASTE", ALAS_KEY_PASTE},   /**< AC Paste */
            {"KEY_FIND", ALAS_KEY_FIND},    /**< AC Find */
            {"KEY_MUTE", ALAS_KEY_MUTE},
            {"KEY_VOLUMEUP", ALAS_KEY_VOLUMEUP},
            {"KEY_VOLUMEDOWN", ALAS_KEY_VOLUMEDOWN},
            {"KEY_KP_COMMA", ALAS_KEY_KP_COMMA},
            {"KEY_KP_EQUALSAS400", ALAS_KEY_KP_EQUALSAS400},
            {"KEY_INTERNATIONAL1", ALAS_KEY_INTERNATIONAL1},
            {"KEY_INTERNATIONAL2", ALAS_KEY_INTERNATIONAL2},
            {"KEY_INTERNATIONAL3", ALAS_KEY_INTERNATIONAL3}, /**< Yen */
            {"KEY_INTERNATIONAL4", ALAS_KEY_INTERNATIONAL4},
            {"KEY_INTERNATIONAL5", ALAS_KEY_INTERNATIONAL5},
            {"KEY_INTERNATIONAL6", ALAS_KEY_INTERNATIONAL6},
            {"KEY_INTERNATIONAL7", ALAS_KEY_INTERNATIONAL7},
            {"KEY_INTERNATIONAL8", ALAS_KEY_INTERNATIONAL8},
            {"KEY_INTERNATIONAL9", ALAS_KEY_INTERNATIONAL9},
            {"KEY_LANG1", ALAS_KEY_LANG1}, /**< Hangul/English toggle */
            {"KEY_LANG2", ALAS_KEY_LANG2}, /**< Hanja conversion */
            {"KEY_LANG3", ALAS_KEY_LANG3}, /**< Katakana */
            {"KEY_LANG4", ALAS_KEY_LANG4}, /**< Hiragana */
            {"KEY_LANG5", ALAS_KEY_LANG5}, /**< Zenkaku/Hankaku */
            {"KEY_LANG6", ALAS_KEY_LANG6}, /**< reserved */
            {"KEY_LANG7", ALAS_KEY_LANG7}, /**< reserved */
            {"KEY_LANG8", ALAS_KEY_LANG8}, /**< reserved */
            {"KEY_LANG9", ALAS_KEY_LANG9}, /**< reserved */
            {"KEY_ALTERASE", ALAS_KEY_ALTERASE},    /**< Erase-Eaze */
            {"KEY_SYSREQ", ALAS_KEY_SYSREQ},
            {"KEY_CANCEL", ALAS_KEY_CANCEL},      /**< AC Cancel */
            {"KEY_CLEAR", ALAS_KEY_CLEAR},
            {"KEY_PRIOR", ALAS_KEY_PRIOR},
            {"KEY_RETURN2", ALAS_KEY_RETURN2},
            {"KEY_SEPARATOR", ALAS_KEY_SEPARATOR},
            {"KEY_OUT", ALAS_KEY_OUT},
            {"KEY_OPER", ALAS_KEY_OPER},
            {"KEY_CLEARAGAIN", ALAS_KEY_CLEARAGAIN},
            {"KEY_CRSEL", ALAS_KEY_CRSEL},
            {"KEY_EXSEL", ALAS_KEY_EXSEL},
            {"KEY_KP_00", ALAS_KEY_KP_00},
            {"KEY_KP_000", ALAS_KEY_KP_000},
            {"KEY_THOUSANDSSEPARATOR", ALAS_KEY_THOUSANDSSEPARATOR},
            {"KEY_DECIMALSEPARATOR", ALAS_KEY_DECIMALSEPARATOR},
            {"KEY_CURRENCYUNIT", ALAS_KEY_CURRENCYUNIT},
            {"KEY_CURRENCYSUBUNIT", ALAS_KEY_CURRENCYSUBUNIT},
            {"KEY_KP_LEFTPAREN", ALAS_KEY_KP_LEFTPAREN},
            {"KEY_KP_RIGHTPAREN", ALAS_KEY_KP_RIGHTPAREN},
            {"KEY_KP_LEFTBRACE", ALAS_KEY_KP_LEFTBRACE},
            {"KEY_KP_RIGHTBRACE", ALAS_KEY_KP_RIGHTBRACE},
            {"KEY_KP_TAB", ALAS_KEY_KP_TAB},
            {"KEY_KP_BACKSPACE", ALAS_KEY_KP_BACKSPACE},
            {"KEY_KP_A", ALAS_KEY_KP_A},
            {"KEY_KP_B", ALAS_KEY_KP_B},
            {"KEY_KP_C", ALAS_KEY_KP_C},
            {"KEY_KP_D", ALAS_KEY_KP_D},
            {"KEY_KP_E", ALAS_KEY_KP_E},
            {"KEY_KP_F", ALAS_KEY_KP_F},
            {"KEY_KP_XOR", ALAS_KEY_KP_XOR},
            {"KEY_KP_POWER", ALAS_KEY_KP_POWER},
            {"KEY_KP_PERCENT", ALAS_KEY_KP_PERCENT},
            {"KEY_KP_LESS", ALAS_KEY_KP_LESS},
            {"KEY_KP_GREATER", ALAS_KEY_KP_GREATER},
            {"KEY_KP_AMPERSAND", ALAS_KEY_KP_AMPERSAND},
            {"KEY_KP_DBLAMPERSAND", ALAS_KEY_KP_DBLAMPERSAND},
            {"KEY_KP_VERTICALBAR", ALAS_KEY_KP_VERTICALBAR},
            {"KEY_KP_DBLVERTICALBAR", ALAS_KEY_KP_DBLVERTICALBAR},
            {"KEY_KP_COLON", ALAS_KEY_KP_COLON},
            {"KEY_KP_HASH", ALAS_KEY_KP_HASH},
            {"KEY_KP_SPACE", ALAS_KEY_KP_SPACE},
            {"KEY_KP_AT", ALAS_KEY_KP_AT},
            {"KEY_KP_EXCLAM", ALAS_KEY_KP_EXCLAM},
            {"KEY_KP_MEMSTORE", ALAS_KEY_KP_MEMSTORE},
            {"KEY_KP_MEMRECALL", ALAS_KEY_KP_MEMRECALL},
            {"KEY_KP_MEMCLEAR", ALAS_KEY_KP_MEMCLEAR},
            {"KEY_KP_MEMADD", ALAS_KEY_KP_MEMADD},
            {"KEY_KP_MEMSUBTRACT", ALAS_KEY_KP_MEMSUBTRACT},
            {"KEY_KP_MEMMULTIPLY", ALAS_KEY_KP_MEMMULTIPLY},
            {"KEY_KP_MEMDIVIDE", ALAS_KEY_KP_MEMDIVIDE},
            {"KEY_KP_PLUSMINUS", ALAS_KEY_KP_PLUSMINUS},
            {"KEY_KP_CLEAR", ALAS_KEY_KP_CLEAR},
            {"KEY_KP_CLEARENTRY", ALAS_KEY_KP_CLEARENTRY},
            {"KEY_KP_BINARY", ALAS_KEY_KP_BINARY},
            {"KEY_KP_OCTAL", ALAS_KEY_KP_OCTAL},
            {"KEY_KP_DECIMAL", ALAS_KEY_KP_DECIMAL},
            {"KEY_KP_HEXADECIMAL", ALAS_KEY_KP_HEXADECIMAL},
            {"KEY_LCTRL", ALAS_KEY_LCTRL},
            {"KEY_LSHIFT", ALAS_KEY_LSHIFT},
            {"KEY_LALT", ALAS_KEY_LALT}, /**< alt, option */
            {"KEY_LGUI", ALAS_KEY_LGUI}, /**< windows, command (apple), meta */
            {"KEY_RCTRL", ALAS_KEY_RCTRL},
            {"KEY_RSHIFT", ALAS_KEY_RSHIFT},
            {"KEY_RALT", ALAS_KEY_RALT}, /**< alt gr, option */
            {"KEY_RGUI", ALAS_KEY_RGUI}, /**< windows, command (apple), meta */
            {"KEY_MODE", ALAS_KEY_MODE},
            {"KEY_SLEEP", ALAS_KEY_SLEEP},                   /**< Sleep */
            {"KEY_WAKE", ALAS_KEY_WAKE},                    /**< Wake */
            {"KEY_CHANNEL_INCREMENT", ALAS_KEY_CHANNEL_INCREMENT},       /**< Channel Increment */
            {"KEY_CHANNEL_DECREMENT", ALAS_KEY_CHANNEL_DECREMENT},       /**< Channel Decrement */
            {"KEY_MEDIA_PLAY", ALAS_KEY_MEDIA_PLAY},          /**< Play */
            {"KEY_MEDIA_PAUSE", ALAS_KEY_MEDIA_PAUSE},         /**< Pause */
            {"KEY_MEDIA_RECORD", ALAS_KEY_MEDIA_RECORD},        /**< Record */
            {"KEY_MEDIA_FAST_FORWARD", ALAS_KEY_MEDIA_FAST_FORWARD},  /**< Fast Forward */
            {"KEY_MEDIA_REWIND", ALAS_KEY_MEDIA_REWIND},        /**< Rewind */
            {"KEY_MEDIA_NEXT_TRACK", ALAS_KEY_MEDIA_NEXT_TRACK},    /**< Next Track */
            {"KEY_MEDIA_PREVIOUS_TRACK", ALAS_KEY_MEDIA_PREVIOUS_TRACK}, /**< Previous Track */
            {"KEY_MEDIA_STOP", ALAS_KEY_MEDIA_STOP},          /**< Stop */
            {"KEY_MEDIA_EJECT", ALAS_KEY_MEDIA_EJECT},         /**< Eject */
            {"KEY_MEDIA_PLAY_PAUSE", ALAS_KEY_MEDIA_PLAY_PAUSE},    /**< Play / Pause */
            {"KEY_MEDIA_SELECT", ALAS_KEY_MEDIA_SELECT},        /* Media Select */
            {"KEY_AC_NEW", ALAS_KEY_AC_NEW},              /**< AC New */
            {"KEY_AC_OPEN", ALAS_KEY_AC_OPEN},             /**< AC Open */
            {"KEY_AC_CLOSE", ALAS_KEY_AC_CLOSE},            /**< AC Close */
            {"KEY_AC_EXIT", ALAS_KEY_AC_EXIT},             /**< AC Exit */
            {"KEY_AC_SAVE", ALAS_KEY_AC_SAVE},             /**< AC Save */
            {"KEY_AC_PRINT", ALAS_KEY_AC_PRINT},            /**< AC Print */
            {"KEY_AC_PROPERTIES", ALAS_KEY_AC_PROPERTIES},       /**< AC Properties */
            {"KEY_AC_SEARCH", ALAS_KEY_AC_SEARCH},           /**< AC Search */
            {"KEY_AC_HOME", ALAS_KEY_AC_HOME},             /**< AC Home */
            {"KEY_AC_BACK", ALAS_KEY_AC_BACK},             /**< AC Back */
            {"KEY_AC_FORWARD", ALAS_KEY_AC_FORWARD},          /**< AC Forward */
            {"KEY_AC_STOP", ALAS_KEY_AC_STOP},             /**< AC Stop */
            {"KEY_AC_REFRESH", ALAS_KEY_AC_REFRESH},          /**< AC Refresh */
            {"KEY_AC_BOOKMARKS", ALAS_KEY_AC_BOOKMARKS},        /**< AC Bookmarks */
            {"KEY_SOFTLEFT", ALAS_KEY_SOFTLEFT},
            {"KEY_SOFTRIGHT", ALAS_KEY_SOFTRIGHT},
            {"KEY_CALL", ALAS_KEY_CALL}, /**< Used for accepting phone calls. */
            {"KEY_ENDCALL", ALAS_KEY_ENDCALL}, /**< Used for rejecting phone calls. */
            {"KEY_RESERVED", ALAS_KEY_RESERVED},    /**< 400-500 reserved for dynamic keycodes */
            {"KEY_COUNT", ALAS_KEY_COUNT} /**< not a key, just marks the number of scancodes for array bounds */
        }
        );
    }
} // namespace Alas
