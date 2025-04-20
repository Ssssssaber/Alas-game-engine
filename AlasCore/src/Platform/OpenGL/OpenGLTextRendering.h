#pragma once
#include "Renderer/TextRendering.h"
#include "Platform/OpenGL/OpenGLCore.h"
#include <glm.hpp>
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/quaternion.hpp>


namespace Alas
{
    struct Character
    {
        unsigned int TextureID; // ID handle of the glyph texture
        glm::ivec2   Size;      // Size of glyph
        glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
        unsigned int Advance;   // Horizontal offset to advance to next glyph
    };

    class OpenGLTextRendering : public TextRendering
    {
    private:
        std::map<GLchar, Character> Characters;
        unsigned int VAO, VBO;
        unsigned int texture;
        
    public:
        OpenGLTextRendering() = default;
        virtual void Init() override;

        virtual void RenderText(const std::string& text, glm::vec3 position, float rotation, float scale, glm::vec4 color) override;
    };
} // namespace Alas
