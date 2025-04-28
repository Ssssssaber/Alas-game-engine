#pragma once

#include "Renderer/Framebuffer.h"

namespace Alas {

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual uint32_t ReadPixel(uint32_t attachmentIndex, int x, int y) override;

		virtual uint32_t GetColorAttachment() const override { return _textureColorbuffer; }

		virtual const FramebufferSpecification& GetSpecification() const override { return _specification; }
    private:
        unsigned int _entityIdTexture;
        unsigned int _textureColorbuffer;
        
        // GLenum m_DrawBuffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
        
        FramebufferSpecification _specification;
        uint32_t m_RendererID = 0;
	};

}
