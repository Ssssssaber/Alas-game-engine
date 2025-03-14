#include "OpenGLTexture.h"

#include "OpenGLCore.h"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Alas
{
    OpenGLTexture::OpenGLTexture(const std::string& filepath) : Texture()
    {
        stbi_set_flip_vertically_on_load(1);
        stbi_uc* data = stbi_load(filepath.c_str(), &_width, &_height, &_numChannels, 4);
        
        if (!data)
        {
            ALAS_CORE_ERROR("Failed to load texture");
            return;
        }

        GlCall(glGenTextures(1, &_rendererId));
        GlCall(glBindTexture(GL_TEXTURE_2D, _rendererId));

        GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

        GlCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));

        GlCall(glBindTexture(GL_TEXTURE_2D, 0));
        stbi_image_free(data);
    }

    OpenGLTexture::~OpenGLTexture()
    {
        GlCall(glDeleteTextures(1, &_rendererId));
    }

    
    void OpenGLTexture::Bind(uint32_t slot) const
    {
        GlCall(glActiveTexture(GL_TEXTURE0 + slot));
        GlCall(glBindTexture(GL_TEXTURE_2D, _rendererId));
    }
} // namespace Alas
