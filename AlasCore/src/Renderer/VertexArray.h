#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "SDL3/SDL.h"
namespace Alas {

    class VertexArray
    {
    public:
        VertexArray() { }
        virtual ~VertexArray() {};
        virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
        
        virtual void AddVertexBuffer(const Shared<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const Shared<IndexBuffer>& indexBuffer) = 0;
		virtual const std::vector<Shared<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const Shared<IndexBuffer>& GetIndexBuffer() const = 0;

		static VertexArray* Create();

    protected:
        uint32_t _rendererId;

    };
}