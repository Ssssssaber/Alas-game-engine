#include "OpenGLTextRendering.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/quaternion.hpp>

namespace Alas
{
    void OpenGLTextRendering::Init()
    {              

        FT_Library ft;
        // All functions return a value different than 0 whenever an error occurred
        if (FT_Init_FreeType(&ft))
        {
            ALAS_CORE_ERROR("ERROR::FREETYPE: Could not init FreeType Library");
        }

        // find path to font
        std::string font_name = "Assets/Fonts/Antonio-Bold.ttf";
        if (font_name.empty())
        {               
            ALAS_CORE_ERROR("ERROR::FREETYPE: Failed to load font_name");
        }
        
        // load font as face
        FT_Face face;
        if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
            ALAS_CORE_ERROR("ERROR::FREETYPE: Failed to load font");
        }
        else {
            // set size to load glyphs as
            FT_Set_Pixel_Sizes(face, 0, 48);

            // disable byte-alignment restriction
            GlCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

            // load first 128 characters of ASCII set
            for (unsigned char c = 0; c < 128; c++)
            {
                // Load character glyph 
                if (FT_Load_Char(face, c, FT_LOAD_RENDER))
                {
                    ALAS_CORE_ERROR("ERROR::FREETYTPE: Failed to load Glyph");
                    continue;
                }
                // generate texture
                
                GlCall(glGenTextures(1, &texture));
                GlCall(glGenTextures(1, &texture));
                GlCall(glBindTexture(GL_TEXTURE_2D, texture));
                GlCall(glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_RED,
                    face->glyph->bitmap.width,
                    face->glyph->bitmap.rows,
                    0,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    face->glyph->bitmap.buffer
                ));
                // set texture options
                GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
                GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
                GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
                GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
                // now store character for later use
                Character character = {
                    texture,
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    static_cast<unsigned int>(face->glyph->advance.x)
                };
                Characters.insert(std::pair<char, Character>(c, character));
            }
            GlCall(glBindTexture(GL_TEXTURE_2D, 0));
        }
        // destroy FreeType once we're finished
        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        
        // configure VAO/VBO for texture quads
        // -----------------------------------
        GlCall(glGenVertexArrays(1, &VAO));
        GlCall(glGenBuffers(1, &VBO));
        GlCall(glBindVertexArray(VAO));
        GlCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
        GlCall(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW));
        GlCall(glEnableVertexAttribArray(0));
        GlCall(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0));
        GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GlCall(glBindVertexArray(0));
    }

    void OpenGLTextRendering::RenderOverlayText(const std::string& text, glm::vec3 position, const glm::vec2& scale)
    {
        // activate corresponding render state	
        GlCall(glActiveTexture(GL_TEXTURE0));
        GlCall(glBindTexture(GL_TEXTURE_2D, texture));
        GlCall(glBindVertexArray(VAO));

        // iterate through all characters
        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++) 
        {
            Character ch = Characters[*c];

            float xpos = position.x + ch.Bearing.x * scale.x;
            float ypos = position.y - (ch.Size.y - ch.Bearing.y) * scale.y;

            float w = ch.Size.x * scale.x;
            float h = ch.Size.y * scale.y;
            // update VBO for each character
            float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },            
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }           
            };
            // render glyph texture over quad
            GlCall(glBindTexture(GL_TEXTURE_2D, ch.TextureID));
            // update content of VBO memory
            GlCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
            GlCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices)); // be sure to use glBufferSubData and not glBufferData

            GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
            // render quad
            GlCall(glDrawArrays(GL_TRIANGLES, 0, 6));
            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            position.x += (ch.Advance >> 6) * scale.x; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
        }
        GlCall(glBindVertexArray(0));
        GlCall(glBindTexture(GL_TEXTURE_2D, 0));
    }

    void OpenGLTextRendering::RenderWorldspaceText(const std::string& text, const glm::vec2& scale)
    {
        glm::vec3 position = glm::vec3(0.0f);
        // worldspace position of a text was already taken into account in shader as a uniform
        RenderOverlayText(text, position, scale);
    }
} // namespace Alas
