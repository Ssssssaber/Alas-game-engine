#pragma once

#include "Platform/OpenGL/OpenGLCore.h"

#include "Shader.h"
#include "VertexArray.h"

namespace Alas
{
    struct FramebufferSpecification 
    {
        int Width, Height;
        uint32_t Samples = 1;
        
        bool IsSwapChainTarget = false;
    };

	class Framebuffer
	{
	public:
        static Shared<Framebuffer> Create(const FramebufferSpecification& spec);
        virtual ~Framebuffer() = default;

        virtual const FramebufferSpecification& GetSpecification() const = 0;
        virtual uint32_t GetColorAttachment() const = 0;

        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        virtual void Resize(uint32_t width, uint32_t height) = 0;
        // void DrawBuffers();
        virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;
    };
} // namespace Alas
