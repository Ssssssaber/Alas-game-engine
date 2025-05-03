#pragma once
#include "Renderer/TextRendering.h"
#include "Platform/OpenGL/OpenGLCore.h"
#include <map>
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

        virtual void RenderOverlayText(const std::string& text, glm::vec3 position, const glm::vec2& scale) override;
        virtual void RenderWorldspaceText(const std::string& text, const glm::vec2& scale) override;
    };
} // namespace Alas
