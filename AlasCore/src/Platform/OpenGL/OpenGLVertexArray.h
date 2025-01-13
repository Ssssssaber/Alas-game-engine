#pragma once 
#include "Renderer/VertexArray.h"


namespace Alas
{
    class OpenGLVertexArray : public VertexArray
    {
    public:
        OpenGLVertexArray();
        virtual ~OpenGLVertexArray();

        virtual void Bind() const override;
		virtual void Unbind() const override;

        virtual void AddVertexBuffer(const Shared<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const Shared<IndexBuffer>& indexBuffer) override;
		virtual const std::vector<Shared<VertexBuffer>>& GetVertexBuffers() const { return _vertexBuffers; }
		virtual const Shared<IndexBuffer>& GetIndexBuffer() const { return _indexBuffer; }
	private:
		
		std::vector<Shared<VertexBuffer>> _vertexBuffers;
		Shared<IndexBuffer> _indexBuffer;
    };
} // namespace AGS
