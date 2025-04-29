#include "OpenGLFramebuffer.h"

namespace Alas {

    static uint32_t s_MaxFramebufferSize = 8192;

    OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec) :
        _specification(spec)
    {
        UpdateFramebuffer();
    }

    OpenGLFramebuffer::~OpenGLFramebuffer()
    {
        GlCall(glDeleteFramebuffers(1, &m_RendererID));
    }

    
    void OpenGLFramebuffer::UpdateFramebuffer()
    {
        if (m_RendererID)
		{
			GlCall(glDeleteFramebuffers(1, &m_RendererID));
			GlCall(glDeleteTextures(1, &_textureColorbuffer));
            GlCall(glGenTextures(1, &_entityIdTexture));
        }

        GlCall(glGenFramebuffers(1, &m_RendererID));
        GlCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
        // create a color attachment texture
        GlCall(glGenTextures(1, &_textureColorbuffer));
        GlCall(glBindTexture(GL_TEXTURE_2D, _textureColorbuffer));
        GlCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _specification.Width, _specification.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
        GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GlCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _textureColorbuffer, 0));

        
        GlCall(glGenTextures(1, &_entityIdTexture));
        GlCall(glBindTexture(GL_TEXTURE_2D, _entityIdTexture));
        GlCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, _specification.Width, _specification.Height, 0, GL_RED_INTEGER, GL_INT, NULL));
        GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        GlCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, _entityIdTexture, 0));
        
        // GlCall(glDrawBuffers(2, m_DrawBuffers));
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            ALAS_ASSERT(false, "ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
        GlCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    void OpenGLFramebuffer::Bind()
    {
        GlCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
    }

    void OpenGLFramebuffer::Unbind()
    {
        GlCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
    {
        if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
        {
            ALAS_CORE_WARN("Attempting to resize buffer to {0}, {1}", width, height);
            return;
        }
        _specification.Width = width;
        _specification.Height = height;

        UpdateFramebuffer();
    }

    int OpenGLFramebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
    {
        if (x > _specification.Width || y > _specification.Height ||
            x < 0 || y < 0) 
        {
            ALAS_CORE_WARN("Reading pixel out of bounds");
            return 0;
        }
        // return 1;



        // int32_t* colorData = new int32_t[_specification.Width * _specification.Height * 4];
        // GlCall(glReadPixels(0, 0, _specification.Width, _specification.Height, GL_RGBA, GL_UNSIGNED_BYTE, colorData));

        // int32_t* entityIdData = new int32_t[_specification.Width * _specification.Height * 4];
        // GlCall(glReadPixels(0, 0, _specification.Width, _specification.Height, GL_RED_INTEGER, GL_INT, entityIdData));
        
        // int pixelIndex = (y * _specification.Width + x); // Calculate index
        // int selectedEntityId = colorData[pixelIndex];



        // // // GlCall(glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex));
		// // // uint32_t pixelData;
		// // // GlCall(glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData));
        
        // return selectedEntityId;

        // GlCall(glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex));
		// int pixelData;
		// GlCall(glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData));
		// return pixelData;

        // Read back data
        GLubyte colorData[4];
        GlCall(glReadBuffer(GL_COLOR_ATTACHMENT0));
        GlCall(glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, colorData));

        GLint entityIdData;
        GlCall(glReadBuffer(GL_COLOR_ATTACHMENT1));
        GlCall(glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &entityIdData));

        // GlCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, _specification.Width, _specification.Height, 0, GL_RED_INTEGER, GL_INT, NULL));
        // GlCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _specification.Width, _specification.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
        
        ALAS_CORE_INFO("ENTITY: {0}; Color: {1} {2} {3}; ({4} {5}) ", entityIdData, colorData[0], colorData[1], colorData[2], x, y);
        return entityIdData;
    }


    

}

