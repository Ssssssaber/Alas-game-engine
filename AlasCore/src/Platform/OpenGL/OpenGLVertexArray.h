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

        virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;
		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const { return _vertexBuffers; }
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return _indexBuffer; }
	private:
		
		std::vector<std::shared_ptr<VertexBuffer>> _vertexBuffers;
		std::shared_ptr<IndexBuffer> _indexBuffer;
    };
} // namespace AGS
