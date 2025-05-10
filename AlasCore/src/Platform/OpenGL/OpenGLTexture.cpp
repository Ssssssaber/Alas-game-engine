#include "OpenGLTexture.h"

#include "OpenGLCore.h"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Alas
{
    OpenGLTexture::OpenGLTexture(UID uid, uint32_t width, uint32_t height)
		: Texture(uid, width, height)
	{
		ALAS_PROFILE_FUNCTION();

		glCreateTextures(GL_TEXTURE_2D, 1, &_rendererId);
		glTextureStorage2D(_rendererId, 1, GL_RGBA8, _width, _height);

		glTextureParameteri(_rendererId, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(_rendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(_rendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(_rendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

    OpenGLTexture::OpenGLTexture(UID uid, const std::string& filepath) : Texture(uid)
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

    void OpenGLTexture::SetData(void* data, uint32_t size) const
	{
		ALAS_PROFILE_FUNCTION();

		uint32_t bpp = 4;
		ALAS_CORE_ASSERT(size == _width * _height * bpp, "Data must be entire texture!");
		glTextureSubImage2D(_rendererId, 0, 0, 0, _width, _height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
    
    void OpenGLTexture::Bind(uint32_t slot) const
    {
        GlCall(glActiveTexture(GL_TEXTURE0 + slot));
        GlCall(glBindTexture(GL_TEXTURE_2D, _rendererId));
    }
} // namespace Alas
