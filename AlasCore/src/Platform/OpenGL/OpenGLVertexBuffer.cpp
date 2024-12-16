#include "OpenGLVertexBuffer.h"
#include "OpenGLCore.h"

namespace Alas {

    OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
	{
		GlCall(glCreateBuffers(1, &_rendererId));
		GlCall(glBindBuffer(GL_ARRAY_BUFFER, _rendererId));
		GlCall(glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW));
	}
	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		GlCall(glDeleteBuffers(1, &_rendererId));
	}
	void OpenGLVertexBuffer::Bind() const
	{
		GlCall(glBindBuffer(GL_ARRAY_BUFFER, _rendererId));
	}
	void OpenGLVertexBuffer::Unbind() const
	{
		GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
}