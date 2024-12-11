#include "Renderer/IndexBuffer.h"

namespace AGS {

    class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

		virtual uint32_t GetCount() const { return _count; }
	private:
		uint32_t _rendererId;
		uint32_t _count;
	};

}