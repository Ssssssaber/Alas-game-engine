#include "OpenGLIndexBuffer.h"
#include "OpenGLCore.h"

namespace AGS
{
    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
    : _count(count)
	{
		GlCall(glCreateBuffers(1, &_rendererId));
		GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererId));
		GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW));
	}
	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		GlCall(glDeleteBuffers(1, &_rendererId));
	}
	void OpenGLIndexBuffer::Bind() const
	{
		GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererId));
	}
	void OpenGLIndexBuffer::Unbind() const
	{
		GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}
} // namespace AGS
