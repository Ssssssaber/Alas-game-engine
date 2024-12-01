#include "Core/Renderer/VertexBuffer.h"

namespace AGS {

    class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

    private:
		uint32_t _rendererId;
	};

}